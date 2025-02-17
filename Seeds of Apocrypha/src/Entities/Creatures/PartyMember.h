#pragma once
#include "Creature.h"

class PartyMember : public Creature {
public:
	bool selected = false;

	PartyMember(const Structure& s, const AnimInfo& a_i, const Animation::Transform& t = {}, const Stats& init_stats = {},
		const string por_name = "Creatures/Portraits/Placeholder", const bool init_biped = true, const bool init_winged = false, const int init_dfc = 0);

	//Game stuff
	void Draw(const bool debug = false) override;

private:
	sf::RectangleShape por_highlight;
};