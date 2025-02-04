#pragma once
#include "Creature.h"

class PartyMember : public Creature {
public:

	PartyMember(const Structure& s, const AnimInfo& a_i, const Animation::Transform& t = {}, const Stats& init_stats = {},
		const bool init_biped = true, const bool init_winged = false, const int init_dfc = 0) :
		Creature(s, a_i, t, init_stats, init_biped, init_winged, init_dfc) {}
};