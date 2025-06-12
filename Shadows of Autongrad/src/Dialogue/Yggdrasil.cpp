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
	int txt_x = game->camera.viewport.x + TS * 2;
	leaf = branch[to_string(dlg_index)];
	string dlg_txt = leaf["Text"];
	text.info.str = dlg_speaker + dlg_txt;
	text.MoveTo({ txt_x, dlg_box.GetPos().y - dlg_box.GetSprSize().y + TS });
	game->renderer.DrawTxt(text);

	//Load, move, and draw the choices
	if (!choices.size())
		LoadChoices();

	for (int i = 0; i < choices.size(); ++i) {
		if (!i)
			choices[i].MoveTo(Round(txt_x, dlg_box.GetPos().y - dlg_box.GetSprSize().y * .5));
		else
			choices[i].MoveTo(Round(txt_x, choices[i - 1].info.pos.y + choices[i - 1].info.str_size.y));
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
	speaker = npc->GetName();
	branch = area_dlg[speaker];
	if (branch.empty()) {
		cerr << "Failed to open dialogue branch for character " << speaker << "\n";
		return;
	}

	branch = branch[npc->dlg_node];
	if (branch.empty()) {
		cerr << "Failed to open dialogue node " << npc->dlg_node << " for character " << speaker << "\n";
		return;
	}
	speaker += ": ";
}

void Yggdrasil::LoadText() {

}

void Yggdrasil::LoadChoices() {
	Text::Info choices_info = {};
	choices_info.max_width = text.info.max_width;
	for (int i = 0; i < leaf["Choices"].size(); ++i) {
		json leaf_choices = leaf["Choices"][to_string(i + 1)];
		
		float alpha = 1;
		if (leaf_choices.contains("Condition")) {
			alpha = CheckCondition(leaf_choices["Condition"]["Check"], leaf_choices["Condition"]["Value"]) ? 1 : .5;
		}
		choices_info.color = Color(1, alpha);
		
		choices_info.str = to_string(i + 1) + ": ";
		choices_info.str += leaf["Choices"][to_string(i + 1)]["Text"];



		choices.push_back(Text(&game->default_fonts[(game->GetResScale() + 1) * 12], choices_info));
	}
}

void Yggdrasil::ChooseDialogue() {
	for (int i = 0; i < choices.size(); ++i) {
		//Can only select valid choices
		if (choices[i].info.color.a == 1) {
			Rect choice_bbox = Rect(choices[i].info.pos, Vector2i(dlg_box.GetSprSize().x - TS * 2, choices[i].info.str_size.y));
			if (Collision::RectPoint(choice_bbox, Input::MousePos())) {
				//Set the color for the selected choice to yellow
				choices[i].info.color = Color(1, 1, 0);
				if (Input::BtnReleased(LMB)) {
					SetIndex(leaf["Choices"][to_string(i + 1)]["Index"]);
					break;
				}
			}
			else
				choices[i].info.color = Color(1);
		}
	}

	if (Input::KeyPressed(SC_1) and choices[0].info.color.a == 1) SetIndex(leaf["Choices"]["1"]["Index"]);
	else if (Input::KeyPressed(SC_2) and choices.size() >= 2 and choices[1].info.color.a == 1) SetIndex(leaf["Choices"]["2"]["Index"]);
	else if (Input::KeyPressed(SC_3) and choices.size() >= 3 and choices[2].info.color.a == 1) SetIndex(leaf["Choices"]["3"]["Index"]);
	else if (Input::KeyPressed(SC_4) and choices.size() >= 4 and choices[3].info.color.a == 1) SetIndex(leaf["Choices"]["4"]["Index"]);
	else if (Input::KeyPressed(SC_5) and choices.size() >= 5 and choices[4].info.color.a == 1) SetIndex(leaf["Choices"]["5"]["Index"]);
}

bool Yggdrasil::CheckCondition(string check, float val) {
	//Different types of checks:
	//-Money (Aeons, float)
	//-Item (Inv, bool)
	//-Disposition (Dispo, int)
	//-Quest flags (Flags, bool)

	if (check == "Aeons") {
		return game->active_scene->CalcPartyAeons() >= val;
	}
	else if (check == "Inv") {
		//Check the party's inventory to see if they have the requisite item
	}
	else if (check == "Dispo") {
		//Check if the speaker's disposition is >= val
	}
	else if (check == "Flags") {
		//Check if all flags have been met
		//This will be... interesting to implement
	}

	return false;
}

void Yggdrasil::CauseEffect(array<string, 3> effects, float val) {
	//Different types of effects:
	//-Alter Party Aeons
	//-Alter Speaker Disposition
	//-Set Quest flags
	
	if (effects[0] == "Aeons") {}
}

void Yggdrasil::SetIndex(int new_index) {
	dlg_index = new_index;
	choices.clear();

	if (!dlg_index) {
		dlg_index = 1;
		game->active_scene->EndDialogue();
	}
}
