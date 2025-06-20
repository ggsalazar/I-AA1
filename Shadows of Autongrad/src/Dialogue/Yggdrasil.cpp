#include "Yggdrasil.h"
#include "../Entities/Creatures/Creature.h"

using std::vector;

void Yggdrasil::Init(Game* g) {
	game = g;

	Sprite::Info i = {};
	i.sheet = "UI/DialogueBox"; i.origin = { .5f, 1.f }; i.scale = game->GetResScale(); i.frame_size = {600, 120};
	dlg_box.Init(i);
	old_dlg_box.Init(i);

	i.sheet = "UI/OldDialogueBtn"; i.origin = { .0f, 1.f }; i.frame_size = { 12, 10 };
	old_dlg_btn.Init(i);

	Text::Info ti = {};
	ti.max_width = dlg_box.GetSprSize().x - TS*4;
	text.Init(&game->default_fonts[(game->GetResScale() + 1) * 12], ti);
	old_text.Init(&game->default_fonts[(game->GetResScale() + 1) * 12], ti);
}

void Yggdrasil::DrawDialogue() {
	//Render the dialogue box and old dialogue "button"
	dlg_box.MoveTo(Vector2f(game->camera.GetCenter().x, game->camera.viewport.y + game->camera.viewport.h));
	game->renderer.DrawSprite(dlg_box);
	
	
	if (!show_old_dlg)
		old_dlg_btn.MoveTo(Vector2f(dlg_box.GetPos().x - dlg_box.GetSprSize().x * .5f, dlg_box.GetPos().y - dlg_box.GetSprSize().y - game->GetResScale()));
	else {
		old_dlg_box.MoveTo(Vector2f(dlg_box.GetPos().x, dlg_box.GetPos().y - dlg_box.GetSprSize().y - game->GetResScale()));
		game->renderer.DrawSprite(old_dlg_box);

		old_dlg_btn.MoveTo(Vector2f(old_dlg_box.GetPos().x - old_dlg_box.GetSprSize().x * .5f, old_dlg_box.GetPos().y - old_dlg_box.GetSprSize().y - game->GetResScale() * 2));
	}
	game->renderer.DrawSprite(old_dlg_btn);

	//Load, move, and draw the dialogue
	while (text.info.str == speaker->GetName() + ": ")
		LoadText(); 
	int text_x = game->camera.viewport.x + TS * 2;
	text.MoveTo({ text_x, dlg_box.GetPos().y - dlg_box.GetSprSize().y + TS });
	game->renderer.DrawTxt(text);

	//Load, move, and draw the choices
	if (!choices.size())
		LoadChoices();

	for (int i = 0; i < choices.size(); ++i) {
		if (!i)
			choices[i].MoveTo(Round(text_x, dlg_box.GetPos().y - dlg_box.GetSprSize().y * .5));
		else
			choices[i].MoveTo(Round(text_x, choices[i - 1].info.pos.y + choices[i - 1].info.str_size.y-8));
		game->renderer.DrawTxt(choices[i]);
	}
}

void Yggdrasil::LoadAreaDialogue(const string& area) {
	//Set dialogue box size
	dlg_box.SetScale(Vector2i(game->GetResScale()));
	text.font = &game->default_fonts[(game->GetResScale() + 1) * 12];
	old_text.font = &game->default_fonts[(game->GetResScale() + 1) * 12];

	//Load json file
	ifstream file("data/Dialogue/"+area+"_dialogue.json");
	if (!file.is_open()) {
		cerr << "Failed to open " << area << " dialogue!/n";
		return;
	}

	file >> area_dlg;
}

void Yggdrasil::LoadNPCDialogue(Creature* npc) {
	speaker = npc;
	text.info.str = speaker->GetName() + ": ";
	trunk = area_dlg[speaker->GetName()];
	if (trunk.empty()) {
		cerr << "Failed to open dialogue branch for character " << speaker->GetName() << "\n";
		return;
	}
	
	LoadBranch();
}

void Yggdrasil::LoadBranch() {
	branch = trunk[speaker->dlg_branch];
	if (branch.empty()) {
		cerr << "Failed to open dialogue node " << speaker->dlg_branch << " for character " << speaker->GetName() << "\n";
		return;
	}
}

void Yggdrasil::LoadText() {
	stem = branch[stem_index];
	text.info.str = speaker->GetName() + ": ";
		
	if (stem.contains("Condition")) {
		leaf = stem["Condition"];

		//Condition passed, load the text
		if ((leaf["Value"].size() == 1 and leaf["Value"].is_number_float() and CheckCondition(leaf["Check"], leaf["Value"], leaf["Comparison"])) or
			(leaf["Value"].size() == 2 and leaf["Value"].is_array() and CheckBetween(leaf["Check"], leaf["Value"])) or
			(leaf["Value"].size() == 1 and leaf["Value"].is_string() and CheckChar(leaf["Check"], leaf["Value"])))
			text.info.str += stem["Text"];
		
		//Condition not passed, go where we're told
		else {
			if (leaf["Effect"] == "Jump") {
				speaker->dlg_branch = leaf["Place"];
				LoadBranch();
				SetIndex(0);
			}
			else if (leaf["Effect"] == "Index")
				SetIndex(leaf["Place"]);
		}
	}
	else
		text.info.str += stem["Text"];
}

void Yggdrasil::LoadChoices() {
	leaf = stem["Choices"];
	choices_info.max_width = text.info.max_width;

	for (int leaf_i = 0; leaf_i < leaf.size(); ++leaf_i) {
		choice = leaf[leaf_i];
		
		float alpha = 1;
		if (choice.contains("Condition")) {
			json cond = choice["Condition"];
			if (cond["Value"].size() == 1 and cond["Value"].is_string() and !CheckChar(cond["Check"], cond["Value"])) alpha = .5;
			else if (cond["Value"].size() == 2 and cond["Value"].is_array() and !CheckBetween(cond["Check"], cond["Value"])) alpha = .5;
			else if (cond["Value"].size() == 1 and cond["Value"].is_number() and !CheckCondition(cond["Check"], cond["Value"], cond["Comparison"])) alpha = .5;
		}
		choices_info.color = Color(1, alpha);
		
		choices_info.str = to_string(leaf_i + 1) + ": ";
		choices_info.str += choice["Text"];

		choices.push_back(Text(&game->default_fonts[(game->GetResScale() + 1) * 12], choices_info));
	}
}

void Yggdrasil::MakeChoice() {
	//Choose a dialogue option
	for (int i = 0; i < choices.size(); ++i) {

		//Can only select valid choices
		if (choices[i].info.color.a == 1) {
			
			Rect choice_bbox = Rect(Vector2i(choices[i].info.pos.x, choices[i].info.pos.y + 5), Vector2i(dlg_box.GetSprSize().x - TS * 2, choices[i].info.str_size.y - 9));
			
			if (Collision::RectPoint(choice_bbox, Input::MousePos())) {
				
				//Set the color for the selected choice to yellow
				choices[i].info.color = Color(1, 1, 0);
				
				if (Input::BtnReleased(LMB)) {
					choice = leaf[i];

					for (int effect_i = 0; effect_i < choice["Effects"].size(); ++effect_i) {
						json effect = choice["Effects"][effect_i];

						if (effect["Effect"] == "Jump") JumpBranch(effect["Value"]);
							
						else CauseEffect(effect["Effect"], effect["Value"]);
					}
				}

			}
			else
				choices[i].info.color = Color(1);
		}
	}

	int choice_num = -1;
	if (Input::KeyPressed(SC1)) choice_num = 0;
	else if (Input::KeyPressed(SC2)) choice_num = 1;
	else if (Input::KeyPressed(SC3)) choice_num = 2;
	else if (Input::KeyPressed(SC4)) choice_num = 3;
	else if (Input::KeyPressed(SC5)) choice_num = 4;
	if (choice_num != -1) MakeChoiceNums(choice_num);



	//Show old dialogue
	Rect old_dlg_rect = Rect(Vector2i(old_dlg_btn.GetPos().x, old_dlg_btn.GetPos().y - old_dlg_btn.GetSprSize().y), old_dlg_btn.GetSprSize());
	if (Collision::RectPoint(old_dlg_rect, Input::MousePos()) and Input::BtnReleased(LMB))
		show_old_dlg = !show_old_dlg;
}

bool Yggdrasil::CheckCondition(const string& check, const float val, const string& comp) {
	//Types of checks:
	//-Money (Aeons, float)
	//-Item (Inv, bool)
	//-Disposition (Dispo, int)
	//-Quest flags (Flags, bool)

	if (check == "Aeons") {
		float a = game->active_scene->GetPartyAeons();
		if (comp == "G") return a > val;
		else if (comp == "GE") return a >= val;
		else if (comp == "E") return a == val;
		else if (comp == "LE") return a <= val;
		else if (comp == "L") return a < val;
	}
	else if (check == "Inv") {
		//Check the party's inventory to see if they have the requisite item
	}
	else if (check == "Disposition") {
		//Check the speaker's party_dispo
		if (comp == "G") return speaker->GetDispo() > val;
		else if (comp == "GE") return speaker->GetDispo() >= val;
		else if (comp == "E") return speaker->GetDispo() == val;
		else if (comp == "LE") return speaker->GetDispo() <= val;
		else if (comp == "L") return speaker->GetDispo() < val;
	}
	else if (check == "Flags") {
		//Check if all flags have been met
		//This will be... interesting to implement
	}

	return false;
}

bool Yggdrasil::CheckChar(const string& check, const string& val) {
	//Checks:
	//-Race
	//-Class
	//-Background

	//For each of these, so long as at least one party member meets the check, they pass
	if (check == "Race") {
		for (const auto& race : game->active_scene->GetPartyRaces()) {
			if (RaceToString(race) == val) return true;
		}
	}
	else if (check == "Class") {
		array<uint, 4> levels = game->active_scene->GetPartyClasses();

		if (val == "Arcanist" and levels[0]) return true;
		if (val == "Rogue" and levels[1]) return true;
		if (val == "Warrior" and levels[2]) return true;
	}
	else if (check == "Background") {
		for (const auto& bg : game->active_scene->GetPartyBackgrounds()) {
			if (bg == val) return true;
		}
	}


	return false;
}

bool Yggdrasil::CheckBetween(string check, array<float, 2> vals) {
	if (check == "Aeons") return min(vals[0], vals[1]) < game->active_scene->GetPartyAeons() and game->active_scene->GetPartyAeons() < max(vals[0], vals[1]);
	else if (check == "Disposition") return min(vals[0], vals[1]) < speaker->GetDispo() and speaker->GetDispo() < max(vals[0], vals[1]);
	return false;
}

void Yggdrasil::CauseEffect(string effect, float val) {
	//Types of effects:
	//-Set stem_index (where we are in current branch)
	//-Alter Party Aeons
	//-Alter Speaker Disposition
	//-Set Quest values
	

	if (effect == "Index") SetIndex(val);
	else if (effect == "Aeons") {
		if (-val > game->active_scene->GetPartyAeons()) game->active_scene->SetPartyAeons(0);
		else game->active_scene->AlterPartyAeons(val);
	}
	else if (effect == "Disposition") speaker->AlterDispo(val);
	else if (effect == "Quest") {}
}

void Yggdrasil::JumpBranch(string val) {
	speaker->dlg_branch = val;
	LoadBranch();
	//Don't need to set the index here because in the json, the index should be set to 0 for any choice that has the "Jump" Effect
}

void Yggdrasil::MakeChoiceNums(int choice_num) {
	if (choices.size() >= choice_num+1 and choices[choice_num].info.color.a == 1) {
		choice = leaf[choice_num];

		for (int effect_i = 0; effect_i < choice["Effects"].size(); ++effect_i) {
			json effect = choice["Effects"][effect_i];

			if (effect["Effect"] == "Jump") JumpBranch(effect["Value"]);

			else CauseEffect(effect["Effect"], effect["Value"]);
		}
	}
}

void Yggdrasil::SetIndex(int new_index) {
	stem_index = new_index;
	text.info.str = speaker->GetName() + ": ";
	choices.clear();

	if (stem_index == -1) {
		stem_index = 0;
		game->active_scene->EndDialogue();
	}
}
