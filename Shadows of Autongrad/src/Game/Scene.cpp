#include "Scene.h"
#include "Interface.h" //Menu (Game)
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

			//Load area stuff
			//Tilemap
			tilemap.Load(game->renderer.GetRenderer(), AreaToString(game->area));
			Vector2u area_size = tilemap.GetMapSizePixels(), area_size_t = tilemap.GetMapSizeTiles();
			//NPCs
			LoadNPCs();
			//Dialogue
			LoadDialogue();


			//Init the grid
			grid.Init(&tilemap);

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
			menus.insert({ Menus::Char_Sheet, new Interface(Menus::Char_Sheet) });
			menus.insert({ Menus::Inv, new Interface(Menus::Inv) });
			menus.insert({ Menus::Journal, new Interface(Menus::Journal) });
			menus.insert({ Menus::Map_Area, new Interface(Menus::Map_Area) });
			menus.insert({ Menus::Map_World, new Interface(Menus::Map_World) });
			menus.insert({ Menus::Options_I, new Interface(Menus::Options_I) });

			//Populate the node grid for pathfinding
			grid.PopulateNodeGrid(&entities);
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
	//Reset lmb_action to true if an interface is not open and we are not in dialogue
	// - the entities will determine if it is false
	lmb_action = interface_open == Menus::NOINTRFC and !in_dlg;

	//Input for the entities
	for (auto& e : entities)
		e->GetInput();

	//Input for the menus
	for (auto& m : menus)
		m.second->GetInput();

	//Always be moving the camera
	MoveCamera();

	//If we're in dialogue, we need to get input for said dialogue (making choices, basically)
	if (in_dlg) {
		game->cursor.SetSheetRow(0); //This is probably temporary
		game->cursor.SetColor(1);
	}
	else {
		if (label == Scenes::AREA) {
			OpenInterface();

			if (!game->paused) {
				SelectPartyMems();

				//How many selected party mems do we have?
				vector<PartyMember*> selected_pms;
				for (int i = 0; i < party_mems.size(); ++i) {
					if (party_mems[i]->selected)
						selected_pms.push_back(party_mems[i]);
				}

				//The LMB, when clicked, performs a variety of functions; which function it ends up performing
				// will depend on what it is pointing at
				//Updating action every 10th of a second for performance
				if (!selected_pms.empty() and lmb_action and game->GetGameFrames() % 6 == 0)
					action = LMBAction(selected_pms);
				else if (selected_pms.empty() or !lmb_action) action = Action::DEFAULT;
				//Change the cursor according to current lmb action
				SetGameCursor(action);

				//Perform the action
				//Need to check if lmb_action is valid because this can still refer to the last stored action, even if one was not stored this frame
				if (Input::BtnPressed(LMB) and lmb_action) {
					//Reset our held action
					held_action = Action::NOACTION;

					//If we found a path, walk the smoothed version of that bitch
					if (!found_paths[0].empty()) {
						for (int i = 0; i < selected_pms.size(); ++i)
							selected_pms[i]->SetPath(grid.SmoothPath(found_paths[i]));

						//If we have an action to perform but can't do it because we aren't close enough yet,
						// hold that action
						held_action = action;
						held_tar = mouse_tar;
					}
					//Else perform the action
					else PerformAction();
				}
			}
			//If the game is paused, the game cursor is the default (at least for now)
			else {
				game->cursor.SetSheetRow(0);
				game->cursor.SetColor(1);
			}
		}
	}
}

void Scene::Update() {
	
	//Every frame:
	//Update entities
	for (auto& e : entities)
		e->Update();

	//Update menus
	for (const auto& m : menus)
		m.second->Update();

	//Update the edge panning rects
	if (label == Scenes::AREA) {
		up_edge.x = game->camera.viewport.x; up_edge.y = game->camera.viewport.y;
		down_edge.x = game->camera.viewport.x; down_edge.y = game->camera.viewport.y + game->camera.viewport.h - TS;
		left_edge.x = game->camera.viewport.x; left_edge.y = game->camera.viewport.y;
		right_edge.x = game->camera.viewport.x + game->camera.viewport.w - TS; right_edge.y = game->camera.viewport.y;
	}

	//If we have a held action and the selected party members aren't moving, that must mean we've reached our target, right?
	//This doesn't work because it will call Perform Action every frame, whereas we only need it to be called once
	if (!game->paused and !in_dlg) {
		uint moving_pms = 0;
		for (const auto& pm : party_mems) {
			if (!pm->GetPath().empty()) ++moving_pms;
		}
		if (held_action != Action::NOACTION and !moving_pms) PerformAction();
	}


	//Repopulate the node grid every 10th of a second (every 6 game frames)
	if (game->GetGameFrames() % 6 == 0 and tilemap.Loaded())
		grid.PopulateNodeGrid(&entities);

	//Sort the entities vector (and possibly Menus map) by dfc value every 6th of a second (every 10 game frames) so that entities of a lower dfc value are drawn
	// last (closest to the camera)
	if (game->GetGameFrames() % 10 == 0) {
		sort(entities.begin(), entities.end(),
			[](const Entity* a, const Entity* b) { return a->sprite.GetDFC() > b->sprite.GetDFC(); });
	}
}

void Scene::Draw() {

	//Draw the tilemap first
	if (tilemap.Loaded())
		game->renderer.DrawTilemap(tilemap);

	//Then draw entities if they can be seen by the camera
	for (auto& e : entities)
		e->Draw();

	//Always draw the party member portraits?
	if (label == Scenes::AREA and !in_dlg) {
		for (const auto& pm : party_mems)
			pm->DrawPortrait();
	}


	//Draw the selection box
	if (selecting)
		game->renderer.DrawRect(selec_box, Color(0, 1, 0, .3), Color(0, 1, 0, .75));

	//Draw dialogue
	if (in_dlg) {
		//game->dlg_mngr.DrawDialogue();
		cout << "In dialogue\n";
	}

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

void Scene::OpenInterface() {
	Menus i = Menus::NOINTRFC;
	if (Input::KeyPressed(C_K)) i = Menus::Char_Sheet;
	else if (Input::KeyPressed(I_K)) i = Menus::Inv;
	else if (Input::KeyPressed(J_K)) i = Menus::Journal;
	else if (Input::KeyPressed(M_K)) i = Menus::Map_Area;
	else if (Input::KeyPressed(O_K)) i = Menus::Options_I;


	if (i != Menus::NOINTRFC) {
		//Is the interface already open?
		bool i_was_open = MenuOpen(i);

		//Close currently open interface
		for (auto& m : menus) {
			if (dynamic_cast<Interface*>(m.second))
				OpenMenu(m.first, false);
		}

		//Open the interface if it wasn't already
		OpenMenu(i, !i_was_open);

		//Set interface_open & paused
		//If i was open, then it is now closed, therefore interface_open should be NOINTRFC
		interface_open = i_was_open ? Menus::NOINTRFC : i;
		game->paused = !i_was_open;
	}
}

void Scene::MoveCamera() {
	bool cam_free = false; //This exists basically solely for debugging
	Vector2i cam_pos = { game->camera.viewport.x, game->camera.viewport.y };
	Vector2i cam_size = { game->camera.viewport.w, game->camera.viewport.h };

	//If we are in dialogue, move the camera to the speaker
	if (in_dlg)
		game->camera.MoveCenterTo(Round(Math::Lerp(Vector2f(game->camera.GetCenter()), cam_tar->GetFloatPos(), .075f)));
	
	//Otherwise move it around as normal
	else if (!game->paused) {
		//If the camera is locked to certain party members,
		// get the average of all of their positions and lerp the camera to there
		//How many party mems is the cam locked to (if any)?
		uint cam_locked_pms = 0;
		Vector2f pos_totals = { 0 };
		for (const auto& p_m : party_mems) {
			if (p_m->cam_locked) {
				++cam_locked_pms;
				pos_totals += p_m->GetFloatPos();
			}
		}
		if (cam_locked_pms) {
			Vector2f pos_avg = pos_totals / cam_locked_pms;

			game->camera.MoveCenterTo(Round(Math::Lerp(Vector2f(game->camera.GetCenter()), pos_avg, .075f)));
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
					if (Collision::RectPoint(up_edge, Input::MousePos()) and cam_pos.y > 0)
						new_cam_offset.y -= game->cam_move_spd;
					else if (Collision::RectPoint(down_edge, Input::MousePos()) and cam_pos.y + cam_size.y < tilemap.GetMapSizePixels().y)
						new_cam_offset.y += game->cam_move_spd;
					if (Collision::RectPoint(left_edge, Input::MousePos()) and cam_pos.x > 0)
						new_cam_offset.x -= game->cam_move_spd;
					else if (Collision::RectPoint(right_edge, Input::MousePos()) and cam_pos.x + cam_size.x < tilemap.GetMapSizePixels().x)
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
					if (Collision::RectPoint(up_edge, Input::MousePos()))
						new_cam_offset.y -= game->cam_move_spd;
					else if (Collision::RectPoint(down_edge, Input::MousePos()))
						new_cam_offset.y += game->cam_move_spd;
					if (Collision::RectPoint(left_edge, Input::MousePos()))
						new_cam_offset.x -= game->cam_move_spd;
					else if (Collision::RectPoint(right_edge, Input::MousePos()))
						new_cam_offset.x += game->cam_move_spd;
				}
			}

			if (new_cam_offset.x != 0 and new_cam_offset.y != 0)
				new_cam_offset /= sqrt2;

			new_cam_offset = new_cam_offset * game->GetResScale();
			game->camera.MoveBy(Round(new_cam_offset));

		}
	}
	//Prevent the camera from moving past the edges of the world (not working for some reason?)
	Vector2f c_p = { (float)game->camera.viewport.x, (float)game->camera.viewport.y };
	Math::Clamp(c_p.x, 0, tilemap.GetMapSizePixels().x - game->camera.viewport.w);
	Math::Clamp(c_p.y, 0, tilemap.GetMapSizePixels().y - game->camera.viewport.h);
	game->camera.viewport.x = c_p.x;
	game->camera.viewport.y = c_p.y;
}

void Scene::SelectPartyMems() {

	//Select party members
	//Click and drag (selection box/area) while holding SHIFT or CTRL
	//	 -SHIFT selects all party mems inside the selection area
	//	 -CTRL deselects all party mems inside the selection area
	if (!in_dlg) {
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
				if (Collision::RectPoint(selec_box, p_m->GetPos())) {
					if (Input::KeyDown(LSHIFT) or Input::KeyDown(RSHIFT))
						p_m->selected = true;
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
}

Action Scene::LMBAction(vector<PartyMember*>& s_pms) {
	//Possible actions:
	//-Move
	//-Melee Attack (Combat only)
	//-Ranged Attack (Combat only)
	//-Pick up an object
	//-Loot a container
	//-Pick/unlock a lock
	//-Open a door
	//-Speak to NPC
	Action action = Action::NOACTION;

	//What is our current mouse target?
	//Reset
	mouse_tar = nullptr;
	Vector2i path_goal = { -1 };
	for (const auto e : entities) {
		if (Collision::RectPoint(e->GetBBox(), Input::MousePos())) {
			mouse_tar = e;
			break;
		}
	}

	//If not pointing at anything, what tile are we currently pointing at?
	if (!mouse_tar) {
		Vector2i tile_pos = { (int)floor(Input::MousePos().x / TS), (int)floor(Input::MousePos().y / TS) };
		Tile curr_tile = {};
		if (tile_pos.x > 0 and tile_pos.y > 0 and tile_pos.x < tilemap.GetMapSizeTiles().x and tile_pos.y < tilemap.GetMapSizeTiles().y)
			curr_tile = tilemap.GetTileData(tile_pos);

		//If we're not looking at a tile, then there is no action to perform
		if (curr_tile.terrain == Terrain::None) return Action::NOACTION;
		
		//Set our goal to the current tile
		path_goal = Input::MousePos();
		action = Action::Move;
	}

	//If pointing at a creature...
	else if (Creature* c = dynamic_cast<Creature*>(mouse_tar)) {
		if (!dynamic_cast<PartyMember*>(c)) {
			//First, we need to know the target creature's disposition to the party
			int creature_dispo = 0;
			creature_dispo = c->GetDispo();

			//Outright Hostile - Attack it
			if (creature_dispo <= 20) {
				//Attack actions (Melee/Ranged/Spell)
				//Melee Attack (LMB action in combat only)
				//-When mouse is on an enemy...
				//	--in melee range OR
				//	--acting party member ONLY has melee weapon(s) equipped AND can reach chosen enemy (if either condition not met, use visual signifier to show that)
				//		---If > 1 melee weapon equipped, will have to ask which weapon to attack with

				//Ranged Attack (LMB action in combat only)
				//-When mouse is on an enemy...
				//	--Outside of melee range AND acting party member has >= 1 ranged weapon equipped (will have to choose which weapon to use)
			}
			//Testy or better - Speak
			else {
				//Set the creature's position as our goal; the pathfinding algorithm will determine if we are
				// unable to find a path for any reason
				path_goal = c->GetPos();

				action = Action::Talk;
			}
		}
	}

	//Else if pointing at an item/container/door/passage... TO-DO
	//Pick up an object
	//-When mouse is on an object that can be picked up

	//Loot a container
	//-When mouse is on an unlocked container

	//Pick/unlock a lock
	//-When mouse is on a locked door/container

	//Open a door
	//-When mouse is on an unlocked door


	//Set the paths to the target
	if (path_goal != Vector2i(-1)) {
		for (int i = 0; i < s_pms.size(); ++i) {
			//The actual goal node for each individual p_m is determined inside FindPath
			found_paths[i] = grid.FindPath(s_pms[i]->GetPos(), path_goal, mouse_tar);
			//If a path could not be found and we are not too close, return NOACTION
			if (found_paths[i].empty() and Distance(s_pms[i]->GetPos(), path_goal) > 1.5 * METER) return Action::NOACTION;
		}
	}

	return action;
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

void Scene::PerformAction() {
	Action a = held_action == Action::NOACTION ? action : held_action;
	Entity* t = !held_tar ? mouse_tar : held_tar;

	switch (a) {
		case Action::Talk:
			if (Creature* c = dynamic_cast<Creature*>(t)) {
				//What node are we calling?
				game->dlg_mngr.LoadNPCDialogue(c);
				//We are now in dialogue
				in_dlg = true;
				//Set our camera target
				cam_tar = c;
			}
		break;
	}
}

void Scene::RemoveEntity(Entity* e) {
	for (auto it = entities.begin();it != entities.end(); ++it) {
		if (*it == e) {
			delete* it;
			it = entities.erase(it);
			break;
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
	auto new_party_mem = new PartyMember(info); //The remaining arguments are the defaults
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

	party_mems.push_back(new PartyMember(info,
		Creature::Stats{ name, genus, race, size, clss, level, sex, str, con, dex, agi, intl, wis, cha } //The rest are defaults and handled in Initialization
	));
}

void Scene::LoadNPCs() {
	string area = AreaToString(game->area);

	//Get the names of all the NPCs for the area
	ifstream info_file("data/NPC Info/"+area+ "_npc_info.json");
	if (!info_file.is_open()) {
		cerr << "Failed to open list of " << area << " NPCs!\n";
		return;
	}

	json npc_info;
	info_file >> npc_info;

	//Load the data for each npc
	Sprite::Info sprite_info = {};
	string por_name = "Placeholder";
	Creature::Stats stats = {};

	for (auto npc : npc_info["NPCs"]) {
		stats.name = npc["Name"];

		//Sprite information
		sprite_info.sheet = npc["Sprite"]; sprite_info.frame_size = { 24, 48 }; //TEMPORARY
		por_name = npc["Portrait_Sprite"]; sprite_info.pos = Round(npc["Spawn_Pos"]["x"] * TS - TS*.5f, npc["Spawn_Pos"]["y"] * TS - TS*.5f);
		//Stats information
		string genus_str = npc["Genus"]; stats.genus = StringToGenus(genus_str);
		string race_str = npc["Race"]; stats.race = StringToRace(race_str);
		string size_str = npc["Size"]; stats.size = StringToSize(size_str);
		stats.sex = npc["Sex"];
		string class_str = npc["Class"]; stats.clss = StringToClass(class_str);
		stats.level = npc["Level"];
		stats.str = npc["A_Scores"]["STR"]; stats.con = npc["A_Scores"]["CON"]; stats.agi = npc["A_Scores"]["AGI"]; stats.dex = npc["A_Scores"]["DEX"];
		stats.intl = npc["A_Scores"]["INT"]; stats.wis = npc["A_Scores"]["WIS"]; stats.cha = npc["A_Scores"]["CHA"];
		
		//These have to be explicitly extracted from the json
		int npc_dispo = npc["Disposition"];
		bool npc_biped = npc["Biped"];
		bool npc_winged = npc["Winged"];

		entities.push_back(new Creature(sprite_info, stats, por_name, npc_dispo, npc_biped, npc_winged));
	}

}

void Scene::LoadDialogue() {
	game->dlg_mngr.LoadAreaDialogue(AreaToString(game->area));
}