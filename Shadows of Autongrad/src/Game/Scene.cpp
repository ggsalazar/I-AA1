#include "Scene.h"
#include "Game.h"
#include "Menu.h"
#include "Interface.h"
#include "../Core/Input.h" //Window
#include "../Core/Math.h"
#include "../Entities/UI/UI.h" //Entity.h
#include "../Entities/Creatures/PartyMember.h"

using json = nlohmann::json;

void Scene::Open(const bool o) {
	open = o;

	if (open) {
		//Each scene is comprised of Menus & Entities
		if (label == Scenes::TITLE) {
			//Clear out all pre-existing entities and party members (likely unnecessary but keeping around jic)
			entities.clear();
			party_mems.clear();

			game->camera.MoveTo({ 0, 0 });
			menus.insert({ Menus::Main, new Menu(Menus::Main) });
			menus[Menus::Main]->Open();
			menus.insert({ Menus::CharCrea, new Menu(Menus::CharCrea) });
			menus.insert({ Menus::Load, new Menu(Menus::Load) });
			menus.insert({ Menus::Options, new Menu(Menus::Options) });

		}

		else if (label == Scenes::AREA) {
			//Initialize the edge rects
			//Up
			up_edge.w = game->camera.viewport.w; up_edge.h = TS;
			//Down
			down_edge.w = up_edge.w; down_edge.h = TS;
			//Left
			left_edge.w = TS; left_edge.h = game->camera.viewport.h;
			//Right
			right_edge.w = TS; right_edge.h = left_edge.h;

			//Import the appropriate tilemap
			string json_file = "DEFAULT";
			switch (game->area) {
				case Area::Debug:
					json_file = "Debug_Room";
					break;

				case Area::Tutton:
					json_file = "Tutton";
					break;
			}
			//Load that bitch
			tilemap.Load(game->renderer.GetRenderer(), json_file);
			Vector2u area_size = tilemap.GetMapSizePixels();
			Vector2u area_size_t = tilemap.GetMapSizeTiles();

			//Init the grid
			grid.Init(&tilemap);

			//Load NPCs
			LoadNPCs(game->area);


			//Set the party and camera
			Vector2f party_ldr_pos;
			switch (game->area) {
				case Area::Debug:
					party_ldr_pos = area_size_t * .5f * TS;
					if (area_size_t.x * .5f == round(area_size_t.x * .5f))
						party_ldr_pos.x += TS * .5f - 1;
					if (area_size_t.y * .5f == round(area_size_t.y * .5f))
						party_ldr_pos.y += TS * .5f - 1;
				break;

				case Area::Tutton:
					party_ldr_pos = area_size_t * .5f * TS;
					if (area_size_t.x * .5f == round(area_size_t.x * .5f))
						party_ldr_pos.x += TS * .5f - 1;
					if (area_size_t.y * .5f == round(area_size_t.y * .5f))
						party_ldr_pos.y += TS * .5f - 1;
				break;
			}

			for (auto& p_m : party_mems) {
				entities.push_back(p_m);
				p_m->SetScene(this);
				switch (p_m->party_position) {
					case 0:
						p_m->MoveTo(party_ldr_pos);
					break;
				}
				p_m->selected = true;
			}

			game->camera.MoveCenterTo(Round(party_ldr_pos));

			//Initialize our interfaces
			menus.insert({ Menus::Char_Sheet, new Interface(Menus::Char_Sheet, Interfaces::Char_Sheet) });
			menus.insert({ Menus::Inventory, new Interface(Menus::Inventory, Interfaces::Inv) });
			menus.insert({ Menus::Journal, new Interface(Menus::Journal, Interfaces::Journal) });
			menus.insert({ Menus::Map_Area, new Interface(Menus::Map_Area, Interfaces::Map_Area) });
			menus.insert({ Menus::Map_World, new Interface(Menus::Map_World, Interfaces::Map_World) });
			menus.insert({ Menus::Options_I, new Interface(Menus::Options_I, Interfaces::Options) });
		}
	}

	//Scene is closed
	else {
		//This also deletes the buttons that belong to each menu
		for (auto& m : menus)
			m.second->Open(false);
		menus.clear();
		//Deletes all the entities in the scene
		entities.clear();
		party_mems.clear();

		//Unload the TileMap
		tilemap.Unload();
	}
}

void Scene::GetInput() {
	//Reset lmb_action to true if an interface is not open - the entities will determine if it is false
	lmb_action = !interface_open;

	//Input for the entities
	for (auto& e : entities) {
		//Only get input for UI elements if the corresponding menu is open
		if (auto ui = dynamic_cast<UI*>(e.get())) {
			if (ui->menu.GetOpen())
				ui->GetInput();
		}
		else
			e->GetInput();
	}

	if (label == Scenes::AREA) {
		OpenInterface();

		if (!game->paused) {
			MoveCamera();
			SelectPartyMems();

			//The LMB, when clicked, performs a variety of functions; which function it ends up performing
			// will depend on what it is pointing at
			//Updating action every 10th of a second for performance reasons
			if (game->GetGameFrames() % 6 == 0 and lmb_action)
				action = LMBAction();
			if (!lmb_action) action = Action::DEFAULT;

			//Change the cursor according to current lmb action
			SetGameCursor(action);
			if (Input::BtnPressed(LMB)) {
				switch (action) {
					case Action::Move:
						for (const auto& p_m : party_mems) {
							if (p_m->selected) {
								p_m->moving = true;
								p_m->SetPath(found_path);
							}
						}
					break;
				}
			}
		}
	}
}

void Scene::Update() {
	
	//Every frame:
	//Update menus
	for (const auto& m : menus)
		m.second->Update();

	//Update entities
	for (auto& e : entities)
		e->Update();

	//Update the edge panning rects
	if (label == Scenes::AREA) {
		up_edge.x = game->camera.viewport.x; up_edge.y = game->camera.viewport.y;
		down_edge.x = game->camera.viewport.x; down_edge.y = game->camera.viewport.y + game->camera.viewport.h - TS;
		left_edge.x = game->camera.viewport.x; left_edge.y = game->camera.viewport.y;
		right_edge.x = game->camera.viewport.x + game->camera.viewport.w - TS; right_edge.y = game->camera.viewport.y;
	}


	//Remove dead entities every 15th of a second (every 4 game frames)
	if (game->GetGameFrames() % 4 == 0) {
		entities.erase(remove_if(entities.begin(), entities.end(),
			[](const s_ptr<Entity>& e) { return !e->alive;}), entities.end());
	}

	//Repopulate the node grid every 10th of a second (every 6 game frames)
	if (game->GetGameFrames() % 6 == 0 and tilemap.Loaded())
		grid.PopulateNodeGrid(&entities);

	//Sort the entities vector (and possibly Menus map) by dfc value every 6th of a second (every 10 game frames) so that entities of a lower dfc value are drawn
	// last (closest to the camera)
	if (game->GetGameFrames() % 10 == 0) {
		sort(entities.begin(), entities.end(),
			[](const s_ptr<Entity>& a, const s_ptr<Entity>& b) { return a->sprite.GetDFC() > b->sprite.GetDFC(); });
	}
}

void Scene::Draw() {

	//Draw the tilemap first
	if (tilemap.Loaded())
		game->renderer.DrawTilemap(tilemap);

	//Then draw entities
	for (auto& e : entities) {
		//Only draw UI elements if the corresponding menu is open
		if (auto ui = dynamic_cast<UI*>(e.get())) {
			if (ui->menu.GetOpen())
				ui->Draw();
		}
		else {
			//Only draw entities if they can be seen by the camera
			if (Collision::AABB(e->GetBBox(), game->camera.viewport))
				e->Draw();
		}
	}

	//Always draw the party member portraits
	if (label == Scenes::AREA) {
		for (const auto& pm : party_mems)
			pm->DrawPortrait();
	}


	//Draw the selection box
	if (selecting)
		game->renderer.DrawRect(selec_box, Color(0, 1, 0, .3), Color(0, 1, 0, .75));

	//This is here for debugging
	if (!found_path.empty() and lmb_action)
		game->renderer.DrawRect(Rect({ found_path.back() }, { 4 }), Color(1));
	//if (tilemap.Loaded())
	//	game->renderer.DrawNodeGrid(grid);

	//Menus are drawn last since UI will always be closest to the camera
	//To solve dfc problem, may have to just give Menus their own dfc
	for (const auto& m : menus)
		m.second->Draw();
}

void Scene::OpenMenu(Menus menu, const bool o) {
	auto m = menus.find(menu);
	if (m != menus.end())
		m->second->Open(o);
	else cout << "Cannot open/close that Menu\n";
}

bool Scene::MenuOpen(Menus menu) {
	auto m = menus.find(menu);
	if (m != menus.end())
		return m->second->GetOpen();

	cout << "That Menu does not exist in this Scene\n";
	return false;
}

void Scene::ResizeMenus() {
	for (const auto& m : menus)
		m.second->Resize();
}

void Scene::OpenInterface(Interfaces intrfc) {
	//Character Sheet
	if (Input::KeyPressed(C_K)) {
		if (interface != Interface::Char_Sheet) {
			//Close currently open interface - TO-DO
			OpenMenu()
			interface_open = Interface::Char_Sheet;
		}
		else interface_open = Interface::NONE;

		//Open the Char_Sheet menu if it isn't already
		OpenMenu(Menus::Char_Sheet, !MenuOpen(Menus::Char_Sheet));
	}
	//Inventory
	else if (Input::KeyPressed(I_K)) {
		if (interface_open != Interface::Inv) {
			//Close whatever other interface was open - TO-DO
			interface_open = Interface::Inv;
		}
		else interface_open = Interface::NONE;

		//Open the Inventory menu if it isn't already
		OpenMenu(Menus::Inventory, !MenuOpen(Menus::Inventory));
	}
	//Journal
	else if (Input::KeyPressed(J_K)) {
		if (interface_open != Interface::Journal) {
			//Close whatever other interface was open - TO-DO
			interface_open = Interface::Journal;
		}
		else interface_open = Interface::NONE;

		//Open the Journal menu if it isn't already
		OpenMenu(Menus::Journal, !MenuOpen(Menus::Journal));
	}
	//Map (Area)
	else if (Input::KeyPressed(M_K)) {
		if (interface_open != Interface::Map_Area) {
			//Close whatever other interface was open - TO-DO
			interface_open = Interface::Map_Area;
		}
		else interface_open = Interface::NONE;

		//Open the Map_Area menu if it isn't already
		OpenMenu(Menus::Map_Area, !MenuOpen(Menus::Map_Area));
	}
	//Map (World)
	//Options
	else if (Input::KeyPressed(O_K)) {
		if (interface_open != Interface::Options) {
			//Close whatever other menu was open - TO-DO
			interface_open = Interface::Options;
		}
		else interface_open = Interface::NONE;

		//Open the Options_I menu if it isn't already
		OpenMenu(Menus::Options_I, !MenuOpen(Menus::Options_I));
	}

	if (interface_open != Interface::NONE) {
		game->paused = true;
		game->cursor.SetSheetRow(0);
	}
	else game->paused = false;
}

void Scene::MoveCamera() {
	//Move the camera
	bool cam_free = false; //This exists basically solely for debugging
	//Can't move the camera if we are at or past the edge
	Vector2i cam_pos = { game->camera.viewport.x, game->camera.viewport.y };
	Vector2i cam_size = { game->camera.viewport.w, game->camera.viewport.h };

	//How many party mems is the cam locked to?
	uint cam_locked_pms = 0;
	Vector2i pos_totals = { 0 };
	for (const auto& p_m : party_mems) {
		if (p_m->cam_locked) {
			++cam_locked_pms;
			pos_totals += p_m->GetPos();
		}
	}

	//If the camera is locked to certain party members,
	// get the average of all of their positions and lerp the camera to there
	if (cam_locked_pms) {
		Vector2i pos_avg = Round(pos_totals / (float)cam_locked_pms);

		game->camera.MoveCenterTo(Round(Math::Lerp(Vector2f(game->camera.GetCenter()), Vector2f(pos_avg), .075f)));
	}
	//Cam not locked to party mems
	else {
		Vector2f new_cam_offset = { 0 };
		if (!cam_free) {
			//Move the camera via arrow/WASD keys
			if ((Input::KeyDown(UP) or Input::KeyDown(W_K)) and cam_pos.y > 0)
				new_cam_offset.y -= game->cam_move_spd;
			else if ((Input::KeyDown(DOWN) or Input::KeyDown(S_K)) and cam_pos.y + cam_size.y < tilemap.GetMapSizePixels().y)
				new_cam_offset.y += game->cam_move_spd;
			if ((Input::KeyDown(LEFT) or Input::KeyDown(A_K)) and cam_pos.x > 0)
				new_cam_offset.x -= game->cam_move_spd;
			else if ((Input::KeyDown(RIGHT) or Input::KeyDown(D_K)) and cam_pos.x + cam_size.x < tilemap.GetMapSizePixels().x)
				new_cam_offset.x += game->cam_move_spd;

			//Move the camera via edge panning
			if (game->edge_panning) {
				if (Collision::Point(Input::MousePos(), up_edge) and cam_pos.y > 0)
					new_cam_offset.y -= game->cam_move_spd;
				else if (Collision::Point(Input::MousePos(), down_edge) and cam_pos.y + cam_size.y < tilemap.GetMapSizePixels().y)
					new_cam_offset.y += game->cam_move_spd;
				if (Collision::Point(Input::MousePos(), left_edge) and cam_pos.x > 0)
					new_cam_offset.x -= game->cam_move_spd;
				else if (Collision::Point(Input::MousePos(), right_edge) and cam_pos.x + cam_size.x < tilemap.GetMapSizePixels().x)
					new_cam_offset.x += game->cam_move_spd;
			}
		}
		else {
			//Move the camera via arrow/WASD keys
			if ((Input::KeyDown(UP) or Input::KeyDown(W_K)))
				new_cam_offset.y -= game->cam_move_spd;
			else if ((Input::KeyDown(DOWN) or Input::KeyDown(S_K)))
				new_cam_offset.y += game->cam_move_spd;
			if ((Input::KeyDown(LEFT) or Input::KeyDown(A_K)))
				new_cam_offset.x -= game->cam_move_spd;
			else if ((Input::KeyDown(RIGHT) or Input::KeyDown(D_K)))
				new_cam_offset.x += game->cam_move_spd;

			//Move the camera via edge panning
			if (game->edge_panning) {
				if (Collision::Point(Input::MousePos(), up_edge))
					new_cam_offset.y -= game->cam_move_spd;
				else if (Collision::Point(Input::MousePos(), down_edge))
					new_cam_offset.y += game->cam_move_spd;
				if (Collision::Point(Input::MousePos(), left_edge))
					new_cam_offset.x -= game->cam_move_spd;
				else if (Collision::Point(Input::MousePos(), right_edge))
					new_cam_offset.x += game->cam_move_spd;
			}
		}

		if (new_cam_offset.x != 0 and new_cam_offset.y != 0)
			new_cam_offset /= sqrt2;

		new_cam_offset = new_cam_offset * game->GetResScale();
		game->camera.MoveBy(Round(new_cam_offset));

	}

	//Prevent the camera from moving past the edges of the world (not working for some reason?)
	Vector2f c_p = { (float)game->camera.viewport.x, (float)game->camera.viewport.y };
	Math::Clamp(c_p.x, 0, tilemap.GetMapSizePixels().y);
	Math::Clamp(c_p.y, 0, tilemap.GetMapSizePixels().y);
	game->camera.viewport.x = c_p.x;
	game->camera.viewport.y = c_p.y;
}

void Scene::SelectPartyMems() {

	//Select party members
	//Click and drag (selection box/area) while holding SHIFT or CTRL
	//	 -SHIFT selects all party mems inside the selection area
	//	 -CTRL deselects all party mems inside the selection area

	if (Input::BtnPressed(LMB) and (Input::KeyDown(LSHIFT) or Input::KeyDown(RSHIFT) or Input::KeyDown(LCTRL) or Input::KeyDown(RCTRL))) {
		selec_box.x = Input::MousePos().x;
		selec_box.y = Input::MousePos().y;
		selecting = true;
		lmb_action = false;
		//Change cursor sprite to indicate we are now selecting - TO-DO
	}
	else if (Input::BtnReleased(LMB) and selecting) {
		selecting = false;
		lmb_action = true;

		for (auto& p_m : party_mems) {
			if (Collision::Point(p_m->GetPos(), selec_box)) {
				if (Input::KeyDown(LSHIFT) or Input::KeyDown(RSHIFT)) p_m->selected = true;
				else if (Input::KeyDown(LCTRL) or Input::KeyDown(RCTRL)) p_m->selected = false;
			}
			else if (Input::KeyDown(LCTRL) or Input::KeyDown(RCTRL)) p_m->selected = false;
		}

	}
	if (selecting) {
		lmb_action = false;
		//Selection area w/h
		selec_box.w = Input::MousePos().x - selec_box.x;
		selec_box.h = Input::MousePos().y - selec_box.y;

		//Use visual signifiers to indicate which party members are about to be selected - TO-DO
	}
}

Action Scene::LMBAction() {
	//Possible actions:
	//-Move
	//-Melee Attack (Combat only)
	//-Ranged Attack (Combat only)
	//-Pick up an object
	//-Loot a container
	//-Pick/unlock a lock
	//-Open a door
	//-Speak to NPC

	//What is our current mouse target?
	//Reset
	mouse_tar = MouseTarget::None;
	//Are we currently pointing at a creature?
	Creature* c = nullptr;
	for (const auto e : entities) {
		c = dynamic_cast<Creature*>(e.get());
		if (!c) continue;
		if (dynamic_cast<PartyMember*>(c)) continue;
		
		if (Collision::Point(Input::MousePos(), c->GetBBox())) {
			mouse_tar = MouseTarget::Creature;
			break;
		}
	}

	//If not pointing at anything else, what tile are we currently pointing at?
	if (mouse_tar == MouseTarget::None) {
		Vector2i tile_pos = { (int)floor(Input::MousePos().x / TS), (int)floor(Input::MousePos().y / TS) };
		Tile curr_tile = {};
		if (tile_pos.x > 0 and tile_pos.y > 0 and tile_pos.x < tilemap.GetMapSizeTiles().x and tile_pos.y < tilemap.GetMapSizeTiles().y)
			curr_tile = tilemap.GetTileData(tile_pos);

		//If we're not looking at a tile, then there is no action to perform
		if (curr_tile.terrain == Terrain::None) return Action::NOACTION;
		
		
		
		game->renderer.DrawRect(Rect({ tile_pos * TS }, { 4 }), Color(1));

		mouse_tar = MouseTarget::Tile;
	}

	switch (mouse_tar) {
		case MouseTarget::Area_Edge:
		break;

		case MouseTarget::Container:
		break;

		case MouseTarget::Creature: {
			//First, we need to know the target creature's disposition to the party
			int creature_dispo = 0;
			if (c)
				creature_dispo = c->GetDispo();
			if (creature_dispo <= 20) {
				//Attack actions (Melee/Ranged/Spell)
			}
			//If it isn't already outright hostile, default action is to talk to it
			else {
				//If we aren't within speaking range (2 meters), we need to get close enough to speak - TO-DO
				//If we can't get to them, we cannot speak to them and must return NOACTION
				//Else just speak
				return Action::Talk;
			}


			break;
		}

		case MouseTarget::Door:
		break;

		case MouseTarget::Item:
		break;

		case MouseTarget::Passage:
		break;

		case MouseTarget::Tile:
			//Move
			//-For every currently selected party member, calculate a path to the current tile
			//	--if every currently selected party member can reach the area around that tile, return MOVE
			//	--else, return NOACTION
			for (const auto& p_m : party_mems) {
				if (p_m->selected) {

					//Going to have to change the goal node depending on marching order and position of 
					// party_member in said order - TO-DO
					found_path = grid.FindPath(p_m->GetPos(), Input::MousePos());

					//If no path, return no action
					if (found_path.empty())
						return Action::NOACTION;

					//Else, report that we can move
					return Action::Move;
				}
			}
		break;
	}
	

	//Melee Attack (LMB action in combat only)
	//-When mouse is on an enemy...
	//	--in melee range OR
	//	--acting party member ONLY has melee weapon(s) equipped AND can reach chosen enemy (if either condition not met, use visual signifier to show that)
	//		---If > 1 melee weapon equipped, will have to ask which weapon to attack with

	//Ranged Attack (LMB action in combat only)
	//-When mouse is on an enemy...
	//	--Outside of melee range AND acting party member has >= 1 ranged weapon equipped (will have to choose which weapon to use)

	//Pick up an object
	//-When mouse is on an object that can be picked up

	//Loot a container
	//-When mouse is on an unlocked container

	//Pick/unlock a lock
	//-When mouse is on a locked door/container

	//Open a door
	//-When mouse is on an unlocked door

	return Action::NOACTION;
}

void Scene::SetGameCursor(Action action) {
	uint new_row = 0;

	//This method forces me to by personally familiar with where each cursor sprite is in the
	// sheet, which is completely unscalable but works for now
	switch (action) {
		case Action::DEFAULT:
			new_row = 0;
		break;

		case Action::Move:
			new_row = 1;
		break;

		case Action::Talk:
			new_row = 2;
		break;

		case Action::NOACTION:
			new_row = 3;
		break;
	}
	game->cursor.SetSheetRow(new_row);
	if (new_row == 2) game->cursor.SetColor(Color(0, 1, 0));
	else game->cursor.SetColor(Color(1));
}

void Scene::RemoveEntity(s_ptr<Entity> e) {
	auto dead_e = e.get();
	for (const auto& ent : entities) {
		if (ent.get() == dead_e)
			ent.get()->alive = false;
	}
}

void Scene::RemoveEntity(const string ent_name) {
	for (const auto& e : entities) {
		if (auto c = dynamic_cast<Creature*>(e.get())) {
			if (c->GetName() == ent_name)
				c->alive = false;
		}
	}
}

void Scene::SetEntitySFXVolume(const float new_volume) {
	//for (auto& e : entities)
	//	e->sound.setVolume(new_volume);
}

void Scene::CreatePartyMem() {
	Sprite::Info info = {};
	info.sheet = "Creatures/Sentients/PMPlaceholder";
	info.frame_size = { 24, 48 }; info.origin = { .5f }; info.scale = game->GetResScale();
	auto new_party_mem = make_s<PartyMember>(info); //The remaining arguments are the defaults
	new_party_mem->MoveTo({ game->resolution.x * .75f, game->resolution.y * .5f });
	entities.push_back(new_party_mem);
}

void Scene::CreatePreGen(PreGens p_g) {
	string name = "Default";
	Genus genus = Genus::Sentient;
	Race race = Race::Human;
	Size size = Size::Med;
	Class clss = Class::Warrior;
	string sprite = "Creatures/Sentients/PMPlaceholder";
	Vector2i sprite_size = { 24, 48 };
	uint level = 1;
	bool sex = 0;
	float str = 0;
	float con = 0;
	float dex = 0;
	float agi = 0;
	float intl = 0;
	float wis = 0;
	float cha = 0;

	switch (p_g) {
	case PreGens::Spark: //Automaton Arcanist
		//Don't forget to change sprite!
		name = "Spark";
		race = Race::Automaton;
		clss = Class::Arcanist;
		str = 1.5;
		con = 2.5;
		dex = 3;
		agi = 2;
		intl = 4;
		wis = 3;
		cha = 1;
		break;

	case PreGens::Essek: //Female Kobold Rogue
		//Don't forget to change sprite!
		name = "Essek";
		race = Race::Kobold;
		size = Size::Small;
		clss = Class::Rogue;
		sex = 1;
		str = 1;
		con = 2;
		dex = 4;
		agi = 3;
		intl = 2;
		wis = 2.5;
		cha = 2.5;
		break;

	case PreGens::Dakn: //Male Dwarf Warrior
		//Don't forget to change sprite!
		name = "Dakn";
		race = Race::Dwarf;
		sex = 1;
		str = 4;
		con = 4;
		dex = 2.5;
		agi = 3;
		intl = 1;
		wis = 1;
		cha = 1.5;
		break;
	}
	Sprite::Info info = {};
	info.sheet = sprite; info.frame_size = sprite_size;

	party_mems.push_back(make_s<PartyMember>(info,
		Creature::Stats{ name, genus, race, size, clss, level, sex, str, con, dex, agi, intl, wis, cha } //The rest are defaults and handled in Initialization
	));
}

void Scene::LoadNPCs(Area area) {
	string s_area = "";
	switch (area) {
		case Area::Tutton:
		case Area::Debug:
			s_area = "Tutton";
		break;
	}

	//Get the names of all the NPCs for the area
	ifstream names_file("data/NPCs/"+s_area+"/" + s_area + "_NPCs.json");
	if (!names_file.is_open()) {
		cerr << "Failed to open list of " << s_area << " NPCs!/n";
		return;
	}

	json npc_names;
	names_file >> npc_names;

	//Load the data for each npc
	json npc_info;
	Sprite::Info sprite_info = {};
	string por_name = "Placeholder";
	Creature::Stats stats = {};

	for (string name : npc_names["Names"]) {
		ifstream npc_file("data/NPCs/" + s_area + "/" + name + "_info.json");
		if (!npc_file.is_open()) {
			cerr << "Failed to open file for NPC " + name + "!/n";
			continue;
		}
		
		npc_file >> npc_info;

		//Sprite information
		sprite_info.sheet = npc_info["Sprite"]; sprite_info.frame_size = { 24, 48 }; //TEMPORARY
		por_name = npc_info["Portrait_Sprite"]; sprite_info.pos = { npc_info["Spawn_Pos"]["x"] * TS, npc_info["Spawn_Pos"]["y"] * TS};
		//Stats information
		stats.name = npc_info["Name"]; 
		string genus_str = npc_info["Genus"]; stats.genus = StringToGenus(genus_str);
		string race_str = npc_info["Race"]; stats.race = StringToRace(race_str);
		string size_str = npc_info["Size"]; stats.size = StringToSize(size_str);
		stats.sex = npc_info["Sex"];
		string class_str = npc_info["Class"]; stats.clss = StringToClass(class_str);
		stats.level = npc_info["Level"];
		stats.str = npc_info["A_Scores"]["STR"]; stats.con = npc_info["A_Scores"]["CON"]; stats.agi = npc_info["A_Scores"]["AGI"]; stats.dex = npc_info["A_Scores"]["DEX"];
		stats.intl = npc_info["A_Scores"]["INT"]; stats.wis = npc_info["A_Scores"]["WIS"]; stats.cha = npc_info["A_Scores"]["CHA"];
		int npc_dispo = npc_info["Disposition"];
		bool npc_biped = npc_info["Biped"];
		bool npc_winged = npc_info["Winged"];

		entities.push_back(make_s<Creature>(sprite_info, stats, por_name, npc_dispo, npc_biped, npc_winged));
	}

}

//void Scene::CreateNPC(string name) {}
