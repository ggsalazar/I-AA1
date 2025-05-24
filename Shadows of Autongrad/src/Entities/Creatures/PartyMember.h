#pragma once
#include "Creature.h"

class PartyMember : public Creature {
public:
	bool selected = false;
	bool cam_locked = false;
	uint party_position = 0;

	PartyMember(const Sprite::Info& s_i, const Stats& init_stats = {},
		const string por_name = "Placeholder", const bool init_biped = true, const bool init_winged = false);

	//Game stuff
	void GetInput() override;
	void Update() override;
	void Draw() override;

	void DrawPortrait();

private:
	Rect hlth_bar;
	Text hlth_txt;
	
	//Cam locking
	Sprite cam_sprite;
	Rect cam_sprite_bbox;
};