#include "Yggdrasil.h"
#include "../Entities/Creatures/Creature.h"

void Yggdrasil::Init(Game* g) {
	game = g;

	Sprite::Info i = {};
	i.sheet = "UI/DialogueBox"; i.origin = { .5f, 1.f }; i.scale = game->GetResScale(); i.frame_size = {600, 120};
	dlg_box.Init(i);

	Text::Info ti = {};
	ti.max_width = dlg_box.GetSprSize().x - TS*4;
	//Size of font is dependent on ResScale - TO-DO
	dlg_text.Init(&game->default_fonts[(game->GetResScale()+1)*12], ti);
}

void Yggdrasil::LoadAreaDialogue(const string& area) {
	//Set dialogue box size
	dlg_box.SetScale(Vector2i(game->GetResScale()));
	dlg_text.font = &game->default_fonts[(game->GetResScale() + 1) * 12];

	//Load json file
	ifstream file("data/Dialogue/"+area+"_dialogue.json");
	if (!file.is_open()) {
		cerr << "Failed to open " << area << " dialogue!/n";
		return;
	}

	file >> area_dlg;
}

void Yggdrasil::LoadNPCDialogue(Creature* npc) {
	dlg_speaker = npc->GetName();
	dlg_branch = area_dlg[dlg_speaker];
	if (dlg_branch.empty()) {
		cerr << "Failed to open dialogue branch for character " << dlg_speaker << "\n";
		return;
	}

	dlg_branch = dlg_branch[npc->dlg_node];
	if (dlg_branch.empty()) {
		cerr << "Failed to open dialogue node " << npc->dlg_node << " for character " << dlg_speaker << "\n";
		return;
	}
	dlg_speaker += ": ";
}

void Yggdrasil::LoadChoices() {
	Text::Info choices_info = {};
	choices_info.max_width = dlg_text.info.max_width;
	for (int i = 0; i < dlg_leaf["Choices"].size(); ++i) {
		json leaf_choices = dlg_leaf["Choices"][to_string(i + 1)];
		
		float alpha = 1;
		if (leaf_choices.contains("Conditions")) {
			alpha = CheckCondition(leaf_choices["Conditions"]["Check"], leaf_choices["Conditions"]["Value"]) ? 1 : .5;
		}
		choices_info.color = Color(1, alpha);
		
		choices_info.str = to_string(i + 1) + ": ";
		choices_info.str += dlg_leaf["Choices"][to_string(i + 1)]["Text"];



		dlg_choices.push_back(Text(&game->default_fonts[(game->GetResScale() + 1) * 12], choices_info));
	}
}

void Yggdrasil::ChooseDialogue() {
	for (int i = 0; i < dlg_choices.size(); ++i) {
		//Can only select valid choices
		if (dlg_choices[i].info.color.a == 1) {
			Rect choice_bbox = Rect(dlg_choices[i].info.pos, Vector2i(dlg_box.GetSprSize().x - TS * 2, dlg_choices[i].info.str_size.y));
			if (Collision::RectPoint(choice_bbox, Input::MousePos())) {
				//Set the color for the selected choice to yellow
				dlg_choices[i].info.color = Color(1, 1, 0);
				if (Input::BtnReleased(LMB)) {
					SetIndex(dlg_leaf["Choices"][to_string(i + 1)]["Index"]);
					break;
				}
			}
			else
				dlg_choices[i].info.color = Color(1);
		}
	}

	if (Input::KeyPressed(SC_1) and dlg_choices[0].info.color.a == 1) SetIndex(dlg_leaf["Choices"]["1"]["Index"]);
	else if (Input::KeyPressed(SC_2) and dlg_choices.size() >= 2 and dlg_choices[1].info.color.a == 1) SetIndex(dlg_leaf["Choices"]["2"]["Index"]);
	else if (Input::KeyPressed(SC_3) and dlg_choices.size() >= 3 and dlg_choices[2].info.color.a == 1) SetIndex(dlg_leaf["Choices"]["3"]["Index"]);
	else if (Input::KeyPressed(SC_4) and dlg_choices.size() >= 4 and dlg_choices[3].info.color.a == 1) SetIndex(dlg_leaf["Choices"]["4"]["Index"]);
	else if (Input::KeyPressed(SC_5) and dlg_choices.size() >= 5 and dlg_choices[4].info.color.a == 1) SetIndex(dlg_leaf["Choices"]["5"]["Index"]);
}

void Yggdrasil::DrawDialogue() {
	//Render the dialogue box
	dlg_box.MoveTo(Vector2f(game->camera.GetCenter().x, game->camera.viewport.y + game->camera.viewport.h));
	game->renderer.DrawSprite(dlg_box);

	//Load, move, and draw the dialogue
	int txt_x = game->camera.viewport.x + TS * 2;
	dlg_leaf = dlg_branch[to_string(dlg_index)];
	string dlg_txt = dlg_leaf["Text"];
	dlg_text.info.str = dlg_speaker + dlg_txt;
	dlg_text.MoveTo({ txt_x, dlg_box.GetPos().y - dlg_box.GetSprSize().y + TS});
	game->renderer.DrawTxt(dlg_text);

	//Load, move, and draw the choices
	if (!dlg_choices.size())
		LoadChoices();

	for (int i = 0; i < dlg_choices.size(); ++i) {
		if (!i)
			dlg_choices[i].MoveTo(Round(txt_x, dlg_box.GetPos().y - dlg_box.GetSprSize().y * .5));
		else
			dlg_choices[i].MoveTo(Round(txt_x, dlg_choices[i-1].info.pos.y + dlg_choices[i - 1].info.str_size.y));
		game->renderer.DrawTxt(dlg_choices[i]);
	}
}

bool Yggdrasil::CheckCondition(string check, float val) {
	//Different types of checks:
	//-Money (Aeons, float)
	//-Item (Inv, bool)
	//-Disposition (Dispo, int)
	//-Quest flags (Flags, bool)

	if (check == "Aeons") {
		return game->active_scene->CalcPartyAeons() > val;
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

void Yggdrasil::SetIndex(int new_index) {
	dlg_index = new_index;
	dlg_choices.clear();

	if (!dlg_index) {
		dlg_index = 1;
		game->active_scene->EndDialogue();
	}
}
