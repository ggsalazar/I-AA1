#pragma once
#include <fstream>
#include <nlohmann/json.hpp>
#include "../Graphics/Sprite.h"
#include "../Graphics/Text.h"

using json = nlohmann::json;
using std::string;

class Creature;
class Game;

class Yggdrasil {
public:
	json area_dlg;
	json trunk;
	Creature* speaker;
	json branch;
	int stem_index = 1;
	json stem;
	json leaf;
	Text text;
	string dlg_text;
	Text old_text;
	std::vector<Text> choices;
	Text::Info choices_info = {};

	Sprite dlg_box;

	static inline Game* game = nullptr;

	Yggdrasil() = default;
	void Init(Game* g);

	void DrawDialogue();

	//Forest
	void LoadAreaDialogue(const string& area);

	//Trunk
	void LoadNPCDialogue(Creature* npc);

	//Branch
	void LoadBranch();

	//Stems
	void LoadText();
	void LoadChoices();

	void ChooseDialogue();
	
	bool CheckCondition(const string& check, const float val, const string& comp);
	bool CheckBetween(string check, std::array<float, 2> vals);

	void CauseEffect(string effect, float val);
	void JumpBranch(string val);


private:
	void SetIndex(int new_index);

};