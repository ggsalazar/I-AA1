#pragma once
#include "Creature.h"

class PartyMember : public Creature {
public:
	bool selected = false;

	PartyMember(Game& g, Scene* s, const Sprite::Info& s_i, const Stats& init_stats = {},
		const string por_name = "Placeholder", const bool init_biped = true, const bool init_winged = false);

	//Game stuff
	void GetInput() override;
	void Update() override;
	void Draw() override;

private:
	Rect hlth_bar;
	Rect mssng_hlth_bar; //I don't think I need this
	u_ptr<Text> hlth_txt;
};