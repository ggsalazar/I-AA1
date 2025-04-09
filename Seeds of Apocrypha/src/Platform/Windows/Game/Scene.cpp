#include <algorithm>
#include <nlohmann/json.hpp>
#include "Scene.h"
#include "Menu.h"
#include "Math.h"
#include "../Entities/Entity.h"
#include "../Entities/UI/UI.h"
#include "../Entities/Creatures/PartyMember.h"

void Scene::GetInput() {

	if (label == Scenes::AREA) {
		OpenInterface();
		MoveCamera();

		if (!game.paused) {
			SelectPartyMems();

			//The LMB, when clicked, performs a variety of functions; which function it ends up performing 
			// will depend on what it is pointing at
			//Updating action every 6th of a second for performance reasons
			if (game.GetFramesElapsed() % 10 == 0)
				action = LMBAction();
			//Change the cursor according to current lmb action AND whether or not that action is valid (TO-DO)
			game.cursor = game.cursors[action].get();
			if (Input::KeyPressed(LMB)) {
				switch (action) {
					case Actions::MOVE:
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
		else game.cursor = game.cursors[Actions::DEFAULT].get();
	}

	for (auto& e : entities) {
		//Only get input for UI elements if the corresponding menu is open
		if (auto ui = dynamic_cast<UI*>(e.get())) {
			if (ui->menu.GetOpen())
				ui->GetInput();
		}
		else
			e->GetInput();
	}
}

void Scene::Update() {
	//Update menus
	for (auto& m : menus)
		m.second->Update();

	//Update entities
	for (auto& e : entities)
		e->Update();

	//Remove dead entities
	entities.erase(remove_if(entities.begin(), entities.end(),
					[](const shared_ptr<Entity>& e) { return !e->alive;}),
			entities.end());

	//Sort the entities vector (and possibly Menus map) by dfc value every 6th of a second so that entities of a lower dfc value are drawn
	// last (closest to the camera)
	if (game.GetFramesElapsed() % 10 == 0) {
		sort(entities.begin(), entities.end(), [](const shared_ptr<Entity>& a, const shared_ptr<Entity>& b) { return a->dfc > b->dfc; });
		
		//Also taking this opportunity to repopulate/reset the node grid
		if (label == Scenes::AREA)
			tilemap.PopulateNodeGrid();
	}
}

void Scene::Draw() {

	//Draw the tilemap first
	if (tilemap.Loaded())
		window.draw(tilemap);

	//Then draw entities
	for (auto& e : entities) {
		//Only draw UI elements if the corresponding menu is open
		if (auto ui = dynamic_cast<UI*>(e.get())) {
			if (ui->menu.GetOpen())
				ui->Draw();
		}
		else
			e->Draw();
	}

	//Draw the selection box
	if (selecting)
		window.draw(selec_box);

	//Menus are drawn last since UI will always be closest to the camera
	//To solve dfc problem, may have to just give Menus their own dfc
	for (const auto& m : menus)
		m.second->Draw();


	cout << "Camera Pos: " << game.camera.getCenter() << endl;
}

void Scene::OpenInterface(Interfaces intrfc) {
	//Options Interface
	if (Input::KeyPressed(O_K)) {
		if (interface_open != Interfaces::OPTIONS) {
			interface_open = Interfaces::OPTIONS;
			game.paused = true;
			//Close whatever other menu was open - TO-DO
		}
		else {
			interface_open = Interfaces::NONE;
			game.paused = false;
		}
		//Open the options menu if it isn't already
		OpenMenu(Menus::OPTIONS_G, !MenuOpen(Menus::OPTIONS_G));
	}
}

void Scene::MoveCamera() {
	//Move the camera
	Vector2f cam_pos = game.camera.getCenter();
	Vector2f cam_size = game.camera.getSize();
	cam_pos.x -= cam_size.x * .5f; cam_pos.y -= cam_size.y * .5f;
	//Can't move the camera if we are at or past the edge
	if (!game.cam_locked) {
		Vector2f new_cam_offset = { 0.f, 0.f };
		//Move the camera via arrow/WASD keys
		if ((BUTTONDOWN(UP) or BUTTONDOWN(W_K)) and cam_pos.y > 0)
			new_cam_offset.y -= game.cam_move_spd;
		else if ((BUTTONDOWN(DOWN) or BUTTONDOWN(S_K)) and cam_pos.y + cam_size.y < tilemap.GetMapSizePixels().y)
			new_cam_offset.y += game.cam_move_spd;
		if ((BUTTONDOWN(LEFT) or BUTTONDOWN(A_K)) and cam_pos.x > 0)
			new_cam_offset.x -= game.cam_move_spd;
		else if ((BUTTONDOWN(RIGHT) or BUTTONDOWN(D_K)) and cam_pos.x + cam_size.x < tilemap.GetMapSizePixels().x)
			new_cam_offset.x += game.cam_move_spd;

		//Move the camera via edge panning
		if ((MOUSEPOS_W.y < cam_pos.y + TS and MOUSEPOS_W.y > cam_pos.y) and cam_pos.y > 0)
			new_cam_offset.y -= game.cam_move_spd;
		if ((MOUSEPOS_W.y > cam_pos.y + cam_size.y - TS and MOUSEPOS_W.y < cam_pos.y + cam_size.y) and cam_pos.y + cam_size.y < tilemap.GetMapSizePixels().y)
			new_cam_offset.y += game.cam_move_spd;
		if ((MOUSEPOS_W.x < cam_pos.x + TS and MOUSEPOS_W.x > cam_pos.x) and cam_pos.x > 0)
			new_cam_offset.x -= game.cam_move_spd;
		if ((MOUSEPOS_W.x > cam_pos.x + cam_size.x - TS and MOUSEPOS_W.x < cam_pos.x + cam_size.x) and cam_pos.x + cam_size.x < tilemap.GetMapSizePixels().x)
			new_cam_offset.x += game.cam_move_spd;

		new_cam_offset = { round(new_cam_offset.x), round(new_cam_offset.y) };
		game.camera.move(new_cam_offset);
	}
	//If the camera is locked to the party members, follow them automatically
	//Select which party members to lock the camera to - TO-DO
	//Get the average of all of their positions and lerp the camera to there
	else {
		Vector2i pos_totals = { 0, 0 };
		uint selected_p_ms = 0;
		for (const auto& p_m : party_mems) {
			if (p_m->selected) {
				++selected_p_ms;
				pos_totals += p_m->GetPos();
			}
		}
		Vector2f pos_avg = Vector2f(pos_totals.x / selected_p_ms, pos_totals.y / selected_p_ms);
		

		Math::Clamp(pos_avg.x, -cam_size.x*.5, tilemap.GetMapSizePixels().x + cam_size.x*.5);
		Math::Clamp(pos_avg.y, -cam_size.y*.5, tilemap.GetMapSizePixels().y + cam_size.y*.5);

		pos_avg = { round(pos_avg.x), round(pos_avg.y) };

		game.camera.setCenter(Math::Lerp(game.camera.getCenter(), pos_avg, .1));
	}

	window.setView(game.camera);
}

void Scene::SelectPartyMems() {
	//Select party members
	//Click and drag (selection box/area) while holding SHIFT or CTRL
	//	 -SHIFT selects all party mems inside the selection area
	//	 -CTRL deselects all party mems inside the selection area

	if (Input::KeyPressed(LMB) and (BUTTONDOWN(LSHIFT) or BUTTONDOWN(RSHIFT) or BUTTONDOWN(LCTRL) or BUTTONDOWN(RCTRL))) {
		selec_box.setPosition(window.mapPixelToCoords(MOUSEPOS));
		selecting = true;
		selec_wh = { 0,0 };
		//Change cursor sprite to indicate we are now selecting - TO-DO
	}
	else if (Input::KeyReleased(LMB) and selecting) {
		selecting = false;

		selec_area.position = selec_box.getPosition();
		selec_area.size = selec_box.getSize();

		for (auto& p_m : party_mems) {
			if (selec_area.contains(Vector2f(p_m->GetPos()))) {
				if (BUTTONDOWN(LSHIFT) or BUTTONDOWN(RSHIFT)) p_m->selected = true;
				else if (BUTTONDOWN(LCTRL) or BUTTONDOWN(RCTRL)) p_m->selected = false;
			}
			else if (BUTTONDOWN(LCTRL) or BUTTONDOWN(RCTRL)) p_m->selected = false;
		}

	}
	if (selecting) {
		//Selection area w/h
		selec_wh.x = MOUSEPOS_W.x - selec_box.getPosition().x;
		selec_wh.y = MOUSEPOS_W.y - selec_box.getPosition().y;

		selec_box.setSize(selec_wh);

		//Use visual signifiers to indicate which party members are about to be selected - TO-DO
	}
}

Actions Scene::LMBAction() {
	//Possible actions:
	//-Move
	//-Melee Attack (Combat only?)
	//-Ranged Attack (Combat only?)
	//-Pick up an object
	//-Loot a container
	//-Pick/unlock a lock
	//-Open a door
	//-Speak to NPC

	//Convert mouse coordinates from screen to world
	//What tile are we currently pointing at?
	Vector2u tile_pos = { floor(MOUSEPOS_W.x / TS), floor(MOUSEPOS_W.y / TS) };
	//Current meter pos is tile_pos * 2
	
	//What tile are we currently looking at?
	unique_ptr<Tile> curr_tile;
	if (tile_pos.x > 0 and tile_pos.y > 0 and tile_pos.x < tilemap.GetMapSizeTiles().x and tile_pos.y < tilemap.GetMapSizeTiles().y)
		curr_tile = make_unique<Tile>(tilemap.GetTileData(tile_pos));
	
	//If we're not looking at a tile, then there is no action to perform
	if (!curr_tile) return Actions::NOACTION;

	//Move
	//-For every currently selected party member, calculate a path to the current tile
	//	--if every currently selected party member can reach the area around that tile, return MOVE
	//	--else, return NOACTION
	for (const auto& p_m : party_mems) {
		if (p_m->selected) {

			found_path = tilemap.FindPath(p_m->GetPos(), MOUSEPOS_W, window);

			//If no path, return no action
			if (found_path.empty())
				return Actions::NOACTION;

			//Else, report that we can move
			return Actions::MOVE;
		}
	}

	//Melee Attack (LMB action in combat only?)
	//-When mouse is on an enemy...
	//	--in melee range OR
	//	--acting party member ONLY has melee weapon(s) equipped AND can reach chosen enemy (if either condition not met, use visual signifier to show that)
	//		---If > 1 melee weapon equipped, will have to ask which weapon to attack with
	
	//Ranged Attack (LMB action in combat only?)
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
	
	//Speak to NPC
	//-When mouse is on non-hostile creature

	return Actions::NOACTION;
}

//Most *actual* initialization is handled here
void Scene::Open(const bool o) {
	open = o;
	
	if (open) {
		//Each scene is comprised of Menus & Entities
		if (label == Scenes::TITLE) {
			//Clear out all pre-existing entities and party members (likely unnecessary but keeping around jic)
			entities.clear();
			party_mems.clear();

			game.camera.setCenter({ window.getSize().x * .5f, window.getSize().y * .5f });
			window.setView(game.camera);
			auto menu = make_unique<Menu>(game, window, *this, Menus::MAIN);
			menu->Open();
			menus.insert({ Menus::MAIN, move(menu) });
			menu = make_unique<Menu>(game, window, *this, Menus::CHARCREA);
			menus.insert({ Menus::CHARCREA, move(menu) });
			menu = make_unique<Menu>(game, window, *this, Menus::LOAD);
			menus.insert({ Menus::LOAD, move(menu) });
			menu = make_unique<Menu>(game, window, *this, Menus::OPTIONS);
			menus.insert({ Menus::OPTIONS, move(menu) });

		}

		else if (label == Scenes::AREA) {
			//Import the appropriate tilemap
			string json_file = "DEFAULT";
			switch (game.area) {
				case Areas::DEBUG:
					json_file = "Debug_Room";
				break;

				case Areas::TUTTON:
					json_file = "Tutton";
				break;
			}
			//Load that bitch
			tilemap.load(json_file);

			//Set the camera and party members
			Vector2u area_size = tilemap.GetMapSizePixels();
			game.camera.setCenter({ round(area_size.x * .5f), round(area_size.y * .5f)});
			for (auto& p_m : party_mems) {
				entities.push_back(p_m);
				p_m->SetScene(this);
				p_m->MoveTo(Vector2i(game.camera.getCenter().x - TS*.5, game.camera.getCenter().y - TS*.5));
				p_m->selected = true;
			}
			switch (game.area) {
				case Areas::DEBUG:
				break;

				case Areas::TUTTON:
				break;

			}

			//Set the view
			window.setView(game.camera);

			//Initialize our menus
			auto menu = make_unique<Menu>(game, window, *this, Menus::OPTIONS_G);
			menus.insert({ Menus::OPTIONS_G, move(menu) });


			//Initialize selection box
			selec_box.setFillColor(sf::Color::Color(0, 255, 0, 130));
		}
	}
	//Scene is closed
	else {
		//This also deletes the buttons that belong to each menu
		for (const auto& m : menus)
			m.second->Open(false);
		menus.clear();
		//Deletes all the entities in the scene
		entities.clear();
		party_mems.clear();
	}
}

void Scene::OpenMenu(Menus menu, const bool o) {
	if (o) {
		auto m = menus.find(menu);
		if (m != menus.end())
			m->second->Open();
	}
	else {
		auto m = menus.find(menu);
		if (m != menus.end())
			m->second->Open(false);
	}
}

bool Scene::MenuOpen(Menus menu) {
	auto m = menus.find(menu);
	if (m != menus.end())
		return m->second->GetOpen();

	cout << "That Menu does not exist in this Scene" << endl;
	return false;
}

void Scene::ResizeMenus() {
	for (const auto& m : menus)
		m.second->Resize();
}

void Scene::RemoveEntity(shared_ptr<Entity> e) {
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
	for (auto& e : entities)
		e->sound.setVolume(new_volume);
}

void Scene::CreatePartyMem() {
	uint res_scalar = game.GetResScale();

	auto new_party_mem = make_shared<PartyMember>(
		Engine{game, window, this},
		AnimInfo{"Creatures/Sentients/PMPlaceholder", 32, 64},
		Animation::Transform{ Vector2i(round(window.getSize().x*.75), round(window.getSize().y * .5)), {.5f, .5f}, {res_scalar, res_scalar}}); //The remaining arguments are the defaults

	entities.push_back(new_party_mem);
}

void Scene::CreatePreGen(PreGens p_g) {
	uint res_scalar = game.GetResScale();

	string name = "Default";
	Genuses genus = Genuses::SENTIENT;
	Races race = Races::HUMAN;
	Sizes size = Sizes::MED;
	Classes clss = Classes::WARRIOR;
	string sprite = "Creatures/Sentients/PMPlaceholder";
	uint sprite_w = METER;
	uint sprite_h = 2*METER;
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
		case PreGens::SPARK: //Automaton Arcanist
			//Don't forget to change sprite!
			name = "Spark";
			race = Races::AUTOMATON;
			clss = Classes::ARCANIST;
			str = 1.5;
			con = 2.5;
			dex = 3;
			agi = 2;
			intl = 4;
			wis = 3;
			cha = 1;
		break;

		case PreGens::ESSEK: //Female Kobold Rogue
			//Don't forget to change sprite!
			name = "Essek";
			race = Races::KOBOLD;
			size = Sizes::SMALL;
			clss = Classes::ROGUE;
			sex = 1;
			str = 1;
			con = 2;
			dex = 4;
			agi = 3;
			intl = 2;
			wis = 2.5;
			cha = 2.5;
		break;

		case PreGens::DAKN: //Male Dwarf Warrior
			//Don't forget to change sprite!
			name = "Dakn";
			race = Races::DWARF;
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

	auto pre_gen = make_shared<PartyMember>(
		Engine{ game, window, this },
		AnimInfo{ sprite, sprite_w, sprite_h },
		Animation::Transform{ {0, 0}, {.5f, .95f}, {1, 1} },
		Creature::Stats{ name, genus, race, size, clss, level, sex, str, con, dex, agi, intl, wis, cha } //The rest are defaults and handled in Initialization
		); 

	party_mems.push_back(pre_gen);
}
