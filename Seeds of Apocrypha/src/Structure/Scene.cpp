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

		//Select party members
		//Click and drag (selection box/area)
		//	 -If holding SHIFT, deselects all party mems outside the selection area and only selects the one(s) inside
		
		if (Input::KeyPressed(LMB)) {
			selec_box.setPosition(window.mapPixelToCoords(MOUSEPOS));
			selecting = true;
			//Change cursor sprite to indicate we are now selecting - TO-DO
		}
		else if (Input::KeyReleased(LMB) and selecting) {
			selecting = false;
			selec_wh = { 0,0 };

			selec_area.position = selec_box.getPosition();
			selec_area.size = selec_box.getSize();

			for (auto& p_m : party_mems) {
				if (selec_area.contains(p_m->GetPos())) p_m->selected = true;
				else if (!SHIFTDOWN()) p_m->selected = false;
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
		
		//Move the selected party members
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
				tileset = "Test";
				json_file = "TuttonStore";
			break;
		}

		tilemap.load("assets/Sprites/Environments/TileSets/"+tileset+".png", "assets/Sprites/Environments/TileMaps/" + json_file + ".json");

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

void Scene::CreatePartyMember() {
	float res_scalar = game.GetResScale();

	auto new_party_mem = make_shared<PartyMember>(
		Structure{game, window, this},
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
		case PreGens::SORINA: //Female Elf Arcanist
			//Don't forget to change sprite!
			name = "Sorina";
			race = Races::ELF;
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
		Structure{ game, window, this },
		AnimInfo{ sprite, sprite_w, sprite_h },
		Animation::Transform{ {0.f, 0.f}, {.5f, .5f}, {1.f, 1.f} },
		Creature::Stats{ name, genus, race, size, clss, level, sex, str, con, dex, agi, intl, wis, cha } //The rest are defaults and handled in Initialization
		); 

	party_mems.push_back(pre_gen);
}
