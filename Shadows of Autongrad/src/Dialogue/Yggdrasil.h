#pragma once
#include <fstream>
#include <nlohmann/json.hpp>
#include "../Graphics/Sprite.h"

using json = nlohmann::json;

class Creature;
class Game;

class Yggdrasil {
public:
	json area_dlg;
	json dlg_branch;
	int dlg_index = 1;

	Sprite dlg_box;

	Yggdrasil() = default;
	void Init(Game* game);

	void LoadAreaDialogue(const std::string& area);

	void LoadNPCDialogue(Creature* npc);

	//void DrawDialogue();

};