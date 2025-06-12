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
	json branch;
	int branch_index = 1;
	json leaf;
	int leaf_index = 1;
	Text text;
	Text old_text;
	std::vector<Text> choices;
	string speaker;

	Sprite dlg_box;

	static inline Game* game = nullptr;

	Yggdrasil() = default;
	void Init(Game* g);

	void DrawDialogue();


	void LoadAreaDialogue(const std::string& area);

	void LoadNPCDialogue(Creature* npc);

	void LoadText();
	void LoadChoices();

	void ChooseDialogue();
	
	bool CheckCondition(std::string check, float val);

	void CauseEffect(std::array<std::string, 3> effects, float val);


private:
	void SetIndex(int new_index);

};