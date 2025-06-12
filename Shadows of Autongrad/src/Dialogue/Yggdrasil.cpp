#include "Yggdrasil.h"
#include "../Entities/Creatures/Creature.h"

void Yggdrasil::Init(Game* g) {
	game = g;

	Sprite::Info i = {};
	i.sheet = "UI/DialogueBox"; i.origin = { .5f, 1.f }; i.scale = game->GetResScale(); i.frame_size = {600, 120};
	dlg_box.Init(i);

	Text::Info ti = {};
	ti.max_width = dlg_box.GetSprSize().x - TS*4;
	text.Init(&game->default_fonts[(game->GetResScale() + 1) * 12], ti);
	old_text.Init(&game->default_fonts[(game->GetResScale() + 1) * 12], ti);
}

void Yggdrasil::DrawDialogue() {
	//Render the dialogue box
	dlg_box.MoveTo(Vector2f(game->camera.GetCenter().x, game->camera.viewport.y + game->camera.viewport.h));
	game->renderer.DrawSprite(dlg_box);

	//Load, move, and draw the dialogue
	while (dlg_text == "")
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
			choices[i].MoveTo(Round(text_x, choices[i - 1].info.pos.y + choices[i - 1].info.str_size.y));
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
	dlg_text = "";
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
	stem = branch[to_string(stem_index)]["Texts"];
	dlg_text = "";
	for (int leaf_i = 0; leaf_i < stem.size(); ++leaf_i) {
		leaf = stem[to_string(leaf_i+1)];
		
		if (leaf.contains("Condition")) {
			if (leaf["Condition"]["Value"].size() == 1 and CheckCondition(leaf["Condition"]["Check"], leaf["Condition"]["Value"], leaf["Condition"]["Comparison"]))
				dlg_text += leaf["Text"];
			else if (leaf["Condition"]["Value"].size() == 2 and CheckBetween(leaf["Condition"]["Check"], leaf["Condition"]["Value"]))
				dlg_text += leaf["Text"];
		}
		else if (!leaf.contains("Condition"))
			dlg_text += leaf["Text"];
	}

	text.info.str = speaker->GetName() + ": " + dlg_text;
	if (dlg_text == "" and stem_index < branch.size())
		++stem_index;
}

void Yggdrasil::LoadChoices() {
	stem = branch[to_string(stem_index)]["Choices"];
	choices_info = {};
	choices_info.max_width = text.info.max_width;
	for (int leaf_i = 0; leaf_i < stem.size(); ++leaf_i) {
		leaf = stem[to_string(leaf_i + 1)];
		
		float alpha = 1;
		if (leaf.contains("Condition"))
			alpha = CheckCondition(leaf["Condition"]["Check"], leaf["Condition"]["Value"], leaf["Condition"]["Comparison"]) ? 1 : .5;
		choices_info.color = Color(1, alpha);
		
		choices_info.str = to_string(leaf_i + 1) + ": ";
		choices_info.str += leaf["Text"];

		choices.push_back(Text(&game->default_fonts[(game->GetResScale() + 1) * 12], choices_info));
	}
}

void Yggdrasil::ChooseDialogue() {
	for (int i = 0; i < choices.size(); ++i) {

		//Can only select valid choices
		if (choices[i].info.color.a == 1) {
			
			Rect choice_bbox = Rect(choices[i].info.pos, Vector2i(dlg_box.GetSprSize().x - TS * 2, choices[i].info.str_size.y-1));
			
			if (Collision::RectPoint(choice_bbox, Input::MousePos())) {
				
				//Set the color for the selected choice to yellow
				choices[i].info.color = Color(1, 1, 0);
				
				if (Input::BtnReleased(LMB)) {
					leaf = stem[to_string(i + 1)];

					if (leaf.contains("Effects")) {
						for (int effect_i = 0; effect_i < leaf["Effects"].size(); ++effect_i) {
							json effect = leaf["Effects"][to_string(effect_i + 1)];

							if (effect["Value"].is_string()) JumpBranch(effect["Value"]);
							
							else CauseEffect(effect["Effect"], effect["Value"]);
						}
					}

					//Index should be set to 1 in the json for any choice that has the "Jump" Effect
					SetIndex(leaf["Index"]);

					break;
				}
			}
			else
				choices[i].info.color = Color(1);
		}
	}

	if (Input::KeyPressed(SC_1) and choices[0].info.color.a == 1) SetIndex(stem["1"]["Index"]);
	else if (Input::KeyPressed(SC_2) and choices.size() >= 2 and choices[1].info.color.a == 1) SetIndex(stem["2"]["Index"]);
	else if (Input::KeyPressed(SC_3) and choices.size() >= 3 and choices[2].info.color.a == 1) SetIndex(stem["3"]["Index"]);
	else if (Input::KeyPressed(SC_4) and choices.size() >= 4 and choices[3].info.color.a == 1) SetIndex(stem["4"]["Index"]);
	else if (Input::KeyPressed(SC_5) and choices.size() >= 5 and choices[4].info.color.a == 1) SetIndex(stem["5"]["Index"]);
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

bool Yggdrasil::CheckBetween(string check, array<float, 2> vals) {
	if (check == "Aeons") return min(vals[0], vals[1]) < game->active_scene->GetPartyAeons() and game->active_scene->GetPartyAeons() < max(vals[0], vals[1]);
	else if (check == "Disposition") return min(vals[0], vals[1]) < speaker->GetDispo() and speaker->GetDispo() < max(vals[0], vals[1]);
	return false;
}

void Yggdrasil::CauseEffect(string effect, float val) {
	//Types of effects:
	//-Alter Party Aeons
	//-Alter Speaker Disposition
	//-Set Quest flags
	//-Alter dialogue branch/node
	
	if (effect == "Aeons") {
		if (-val > game->active_scene->GetPartyAeons()) game->active_scene->SetPartyAeons(0);
		else game->active_scene->AlterPartyAeons(val);
	}
	else if (effect == "Disposition")
		speaker->AlterDispo(val);
	else if (effect == "Flags") {}
}

void Yggdrasil::JumpBranch(string val) {
	speaker->dlg_branch = val;
	LoadBranch();
	//Don't need to set the index here because in the json, the index should be set to 1 for any choice that has the "Jump" Effect
}

void Yggdrasil::SetIndex(int new_index) {
	stem_index = new_index;
	dlg_text = "";
	choices.clear();

	if (!stem_index) {
		stem_index = 1;
		game->active_scene->EndDialogue();
	}
}
