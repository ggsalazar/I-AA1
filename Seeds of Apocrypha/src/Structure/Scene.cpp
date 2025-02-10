#include <algorithm>
#include "Scene.h"
#include "Menu.h"
#include "../Entities/Entity.h"
#include "../Entities/UI/UI.h"
#include "../Entities/Creatures/PartyMember.h"

void Scene::GetInput() {
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

	for (auto& e : entities) {
		//Only draw UI elements if the corresponding menu is open
		if (auto ui = dynamic_cast<UI*>(e.get())) {
			if (ui->menu.GetOpen())
				ui->Draw();
		}
		else
			e->Draw();
	}

	//Menus are drawn last since UI will always be closest to the camera
	//To solve dfc problem, may have to just give Menus their own dfc
	for (const auto& m : menus)
		m.second->Draw();
}

//Most *actual* initialization is handled here
void Scene::Open() {
	open = true;
	entities.clear();

	float res_scalar = game.GetResScale();

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
		switch (game.area) {
			case Areas::TUTTON:
				//Import the appropriate tilemap
			break;
		}
	}
}

void Scene::Close() {
	open = false;
	//This also deletes the buttons that belong to each menu except for the button that actually changed the scene,
	// which now belongs to the new scene and is immediately deleted when that scene is opened
	//Should I make an "entities_to_keep" vector?
	for (const auto& m : menus)
		m.second->Open(false);
	menus.clear();
	entities.clear();
}

void Scene::OpenMenu(Menus menu) {
	for (const auto& m : menus) {
		if (m.first == menu) {
			m.second->Open();
			break;
		}
	}
}

bool Scene::MenuOpen(Menus menu) {
	for (const auto& m : menus) {
		if (m.first == menu) {
			return m.second->GetOpen();
			break;
		}
	}
	cout << "That Menu does not exist in this Scene" << endl;
	return false;
}

void Scene::CloseMenu(Menus menu) {
	for (const auto& m : menus) {
		if (m.first == menu) {
			m.second->Open(false);
			break;
		}
	}
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
		AnimInfo{ "Creatures/Sentients/PMPlaceholder", 32, 64 },
		Animation::Transform{ {window.getSize().x * .75f, window.getSize().y * .5f}, {.5f, .5f}, {res_scalar, res_scalar} },
		Stats{ name, genus, race, size, clss, level, sex, str, con, dex, agi, intl, wis, cha }); //The rest are defaults and handled in Initialization

	entities.push_back(pre_gen);
}
