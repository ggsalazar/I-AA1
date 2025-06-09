#pragma once
#include <fstream>
#include <nlohmann/json.hpp>
#include "../Graphics/Sprite.h"
#include "../Graphics/Text.h"

using json = nlohmann::json;

class Creature;
class Game;

class Yggdrasil {
public:
	json area_dlg;
	json dlg_branch;
	json dlg_leaf;
	int dlg_index = 1;
	Text dlg_text;
	std::vector<Text> dlg_choices;

	Sprite dlg_box;

	static inline Game* game = nullptr;

	Yggdrasil() = default;
	void Init(Game* g);

	void LoadAreaDialogue(const std::string& area);

	void LoadNPCDialogue(Creature* npc);

	void LoadChoices();

	void ChooseDialogue();
	void DrawDialogue();


private:
	void SetIndex(int new_index);

};