#include <algorithm>
#include <nlohmann/json.hpp>
#include "Scene.h"
#include "Menu.h"
#include "../Entities/Entity.h"
#include "../Entities/UI/UI.h"
#include "../Entities/Creatures/PartyMember.h"

void Scene::GetInput() {

	if (label == Scenes::AREA) {
		MoveCamera();
		SelectPartyMems();

		//The LMB, when clicked, performs a variety of functions; which function it ends up performing will depend on what it is pointed at
		//Updating action every 10th of a second for performance reasons
		if (game.GetFramesElapsed() % 6 == 0)
			action = LMBAction();
		//Change the cursor according to current lmb action
		game.cursor = game.cursors[action].get();
		if (Input::KeyPressed(LMB)) {
			switch (action) {
				case Actions::MOVE:
					MovePartyMems();
				break;

			}
		}
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
	for (auto& e : entities)
		e->Update();

	//Remove dead entities
	entities.erase(remove_if(entities.begin(), entities.end(),
					[](const shared_ptr<Entity>& e) { return !e->alive;}),
			entities.end());

	//Sort the entities vector (and possibly Menus map) by dfc value every 6th of a second so that entities of a lower dfc value are drawn
	// last (closest to the camera)
	if (game.GetFramesElapsed() % 10 == 0)
		sort(entities.begin(), entities.end(), [](const shared_ptr<Entity>& a, const shared_ptr<Entity>& b) { return a->dfc > b->dfc; });
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
}

void Scene::MoveCamera() {
	//Move the camera
	sf::Vector2f cam_pos = game.camera.getViewport().position;
	sf::Vector2f cam_size = game.camera.getSize();
	//Can't move the camera if we are at or past the edge
	sf::Vector2f new_cam_offset = { 0.f, 0.f };
	//Move the camera via arrow keys
	if (UADOWN() and cam_pos.y > 0)
		new_cam_offset.y -= game.cam_move_spd;
	else if (DADOWN() and cam_pos.y + cam_size.y < tilemap.GetMapSizePixels().y)
		new_cam_offset.y += game.cam_move_spd;
	if (LADOWN() and cam_pos.x > 0)
		new_cam_offset.x -= game.cam_move_spd;
	else if (RADOWN() and cam_pos.x + cam_size.x < tilemap.GetMapSizePixels().x)
		new_cam_offset.x += game.cam_move_spd;

	//Move the camera via edge panning
	if ((MOUSEPOS.y < cam_pos.y + 32 and MOUSEPOS.y > cam_pos.y) and cam_pos.y > 0)
		new_cam_offset.y -= game.cam_move_spd;
	if ((MOUSEPOS.y > cam_pos.y + cam_size.y - 32 and MOUSEPOS.y < cam_pos.y + cam_size.y) and cam_pos.y + cam_size.y < tilemap.GetMapSizePixels().y)
		new_cam_offset.y += game.cam_move_spd;
	if ((MOUSEPOS.x < cam_pos.x + 32 and MOUSEPOS.x > cam_pos.x) and cam_pos.x > 0)
		new_cam_offset.x -= game.cam_move_spd;
	if ((MOUSEPOS.x > cam_pos.x + cam_size.x - 32 and MOUSEPOS.x < cam_pos.x + cam_size.x) and cam_pos.x + cam_size.x < tilemap.GetMapSizePixels().x)
		new_cam_offset.x += game.cam_move_spd;

	game.camera.move(new_cam_offset);
	window.setView(game.camera);
}

void Scene::SelectPartyMems() {
	//Select party members
	//Click and drag (selection box/area) while holding SHIFT or CTRL
	//	 -SHIFT selects all party mems inside the selection area
	//	 -CTRL deselects all party mems inside the selection area

	if (Input::KeyPressed(LMB) and (SHIFTDOWN() or CTRLDOWN())) {
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
			if (selec_area.contains(p_m->GetPos())) {
				if (SHIFTDOWN()) p_m->selected = true;
				else if (CTRLDOWN()) p_m->selected = false;
			}
			else if (!CTRLDOWN()) p_m->selected = false;
		}

	}
	if (selecting) {
		sf::Vector2f mouse_pos = window.mapPixelToCoords(MOUSEPOS);
		//Selection area w/h
		selec_wh.x = mouse_pos.x - selec_box.getPosition().x;
		selec_wh.y = mouse_pos.y - selec_box.getPosition().y;

		selec_box.setSize(selec_wh);

		//Use visual signifiers to indicate which party members are about to be selected - TO-DO
	}
}

void Scene::MovePartyMems() {
	//Move each party member according to current marching order
	party_mems[0]->MoveTo(window.mapPixelToCoords(MOUSEPOS));
}

Actions Scene::LMBAction() {
	//Possible actions:
	//-Move
	//	--When mouse is on a tile that all currently selected party members can reach
	//-Melee Attack
	//	--When mouse is on an enemy...
	//		---in melee range OR
	//		---acting party member ONLY has melee weapon(s) equipped AND can reach chosen enemy (if either condition not met, use visual signifier to show that)
	//			----If > 1 melee weapon equipped, will have to ask which weapon to attack with
	//-Ranged Attack
	//	--When mouse is on an enemy...
	//		---Outside of melee range AND acting party member has >= 1 ranged weapon equipped (will have to choose which weapon to use)
	//-Pick up an object
	//	--When mouse is on an object that can be picked up
	//-Loot a container
	//	--When mouse is on an unlocked container
	//-Pick a lock
	//	--When mouse is on a locked door/container
	//-Open a door
	//	--When mouse is on an unlocked door
	//-Speak to NPC
	//	--When mouse is on non-hostile creature
	//THIS SPACE INTENTIONALLY LEFT BLANK

	//Convert mouse coordinates from screen to world
	sf::Vector2f mouse_world_pos = window.mapPixelToCoords(MOUSEPOS);
	//What tile are we currently pointing at?
	sf::Vector2f tile_pos = { mouse_world_pos.x / 32.f, mouse_world_pos.y / 32.f };
	//Party member positions and distances from given tile
	//vector<float> party_mem_dists;
	//for (int i = 0; i < party_mems.size(); ++i)
	//	party_mem_dists.push_back(sqrt((tile_pos.x - party_mems[i]->GetPos().x) * (tile_pos.x - party_mems[i]->GetPos().x) + (tile_pos.y - party_mems[i]->GetPos().y) * (tile_pos.y - party_mems[i]->GetPos().y)));

	//What tile are we currently looking at?
	unique_ptr<Tile> curr_tile;
	if (tile_pos.x > 0 and tile_pos.y > 0 and tile_pos.x < tilemap.GetMapSizeTiles().x and tile_pos.y < tilemap.GetMapSizeTiles().y)
		curr_tile = make_unique<Tile>(tilemap.GetTileData(tile_pos));
	
	//If we're not looking at a tile, then there is no action to perform
	if (!curr_tile) return Actions::NOACTION;

	//Defaulting to Move FOR NOW
	Actions curr_lmb_action = Actions::MOVE;
	//-Move
	//	--When mouse is on a tile that all currently selected party members can reach
	//For every currently selected party member, calculate a path to the current tile
	//	if every currently selected party member can reach that tile, return Actions::MOVE
	//	else, return Actions::NONE
	for (const auto& p_m : party_mems) {
		if (p_m->selected) {
		}
	}


	//Debug
	if (game.debug) {
		string terr;
		switch (curr_tile->terrain) {
			case Terrains::NORMAL:
				terr = "Normal";
			break;

			case Terrains::WATER:
				terr = "Water";
			break;
		}
		cout << "Tile info: -Terrain: " << terr << " : -Elevation: " << tilemap.GetTileData(tile_pos).elevation << endl;
	}

	return curr_lmb_action;
}

//Most *actual* initialization is handled here
void Scene::Open() {
	open = true;
	
	//Each scene is comprised of Menus & Entities
	if (label == Scenes::TITLE) {
		auto main_menu = make_unique<Menu>(game, window, *this, Menus::MAIN);
		auto char_crea_menu = make_unique<Menu>(game, window, *this, Menus::CHARCREA);
		auto load_menu = make_unique<Menu>(game, window, *this, Menus::LOAD);
		auto options_menu = make_unique<Menu>(game, window, *this, Menus::OPTIONS);

		main_menu->Open();
		menus.insert(make_pair(Menus::MAIN, move(main_menu)));
		menus.insert(make_pair(Menus::CHARCREA, move(char_crea_menu)));
		menus.insert(make_pair(Menus::LOAD, move(load_menu)));
		menus.insert(make_pair(Menus::OPTIONS, move(options_menu)));
	}
	else if (label == Scenes::AREA) {
		for (auto& p_m : party_mems) {
			entities.push_back(p_m);
			p_m->SetScene(this);
		}
		//Selection box stuff
		selec_box.setFillColor(sf::Color::Color(0, 250, 0, 128));

		window.setView(game.camera);
		
		//Import the appropriate tilemap
		string tileset = "DEFAULT";
		string json_file = "DEFAULT";
		switch (game.area) {
			case Areas::TUTTON:
				json_file = "TuttonStore";
			break;
		}

		tilemap.load("assets/Sprites/Environments/TileMaps/" + json_file + ".json");

		//Set the camera location and party members
		switch (game.area) {
			case Areas::TUTTON:
				sf::Vector2f area_size = tilemap.GetMapSizePixels();
				game.camera.setCenter({ area_size.x * .5f, area_size.y * .5f });
				for (auto& p_m : party_mems) {
					p_m->MoveTo(game.camera.getCenter());
					p_m->selected = true;
				}
			break;
		}
	}
}

void Scene::Close() {
	open = false;
	//This also deletes the buttons that belong to each menu except for the button that actually changed the scene,
	// which now belongs to the new scene and is immediately deleted when that scene is opened
	for (const auto& m : menus)
		m.second->Open(false);
	menus.clear();
	entities.clear();
}

void Scene::OpenMenu(Menus menu) {
	auto m = menus.find(menu);
	if (m != menus.end())
		m->second->Open();
}

bool Scene::MenuOpen(Menus menu) {
	auto m = menus.find(menu);
	if (m != menus.end())
		return m->second->GetOpen();

	cout << "That Menu does not exist in this Scene" << endl;
	return false;
}

void Scene::CloseMenu(Menus menu) {
	auto m = menus.find(menu);
	if (m != menus.end())
		m->second->Open(false);
}

void Scene::ResizeMenus() {
	for (const auto& m : menus)
		m.second->Resize();
}

void Scene::SetEntitySFXVolume(float new_volume) {
	for (auto& e : entities)
		e->sound.setVolume(new_volume);
}

void Scene::CreatePartyMem() {
	float res_scalar = game.GetResScale();

	auto new_party_mem = make_shared<PartyMember>(
		Engine{game, window, this},
		AnimInfo{"Creatures/Sentients/PMPlaceholder", 32, 64},
		Animation::Transform{ {window.getSize().x*.75f, window.getSize().y * .5f}, {.5f, .5f}, {res_scalar, res_scalar}}); //The remaining arguments are the defaults

	entities.push_back(new_party_mem);
}

void Scene::CreatePreGen(PreGens p_g) {
	float res_scalar = game.GetResScale();

	string name = "Default";
	Genuses genus = Genuses::SENTIENT;
	Races race = Races::HUMAN;
	Sizes size = Sizes::MED;
	Classes clss = Classes::WARRIOR;
	string sprite = "Creatures/Sentients/PMPlaceholder";
	uint sprite_w = 32;
	uint sprite_h = 64;
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

		case PreGens::ESSEK: //Male Kobold Rogue
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
		Animation::Transform{ {0.f, 0.f}, {.5f, .5f}, {1.f, 1.f} },
		Creature::Stats{ name, genus, race, size, clss, level, sex, str, con, dex, agi, intl, wis, cha } //The rest are defaults and handled in Initialization
		); 

	party_mems.push_back(pre_gen);
}
