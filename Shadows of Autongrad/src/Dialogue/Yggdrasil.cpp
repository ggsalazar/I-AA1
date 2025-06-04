#include "Yggdrasil.h"
#include "../Entities/Creatures/Creature.h"

void Yggdrasil::Init(Game* game) {
	Sprite::Info i = {};
	i.sheet = "UI/DialogueBox"; i.origin = { .5f, 1.f }; i.scale = game->GetResScale(); i.frame_size = {600, 120};
	dlg_box.Init(i);
}

void Yggdrasil::LoadAreaDialogue(const string& area) {
	//Load json file
	ifstream file("data/Dialogue/"+area+"_dialogue.json");
	if (!file.is_open()) {
		cerr << "Failed to open " << area << " dialogue!/n";
		return;
	}

	file >> area_dlg;
}

void Yggdrasil::LoadNPCDialogue(Creature* npc) {
	
	dlg_branch = area_dlg[npc->GetName()];
	if (dlg_branch.empty()) {
		cerr << "Failed to open dialogue branch for character " << npc->GetName() << "\n";
		return;
	}

	dlg_branch = dlg_branch[npc->dlg_node];
	if (dlg_branch.empty()) {
		cerr << "Failed to open dialogue node " << npc->dlg_node << " for character " << npc->GetName() << "\n";
		return;
	}
}