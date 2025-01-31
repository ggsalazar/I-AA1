#include "Creature.h"

Creature::Creature(const Structure& s, const AnimInfo& a_i, const Animation::Transform& t, const Stats& init_stats,
	const bool init_biped, const bool init_winged, const int init_dfc) :
	Entity(s, a_i, t, init_dfc), stats(init_stats), biped(init_biped), winged(init_winged) {
	/*struct Stats {
		string name = "";
		Sizes size = Sizes::TINY;
		Classes clss = Classes::NONE;
		float str = 0;
		float con = 0;
		float dex = 0;
		float agi = 0;
		float intl = 0;
		float wis = 0;
		float cha = 0;
		float max_hlth = 0;
		float hlth = 0;
		float th_per_hlth = 0;
		float tw_per_hlth = 0;
		float tn_per_hlth = 0;
		float dodge = 0;
		float nat_armor = 0;
		float worn_armor = 0;
		float m_def = 0;
		float r_def = 0;
		float fort = 0;
		float ref = 0;
		float will = 0;
		float self_weight = 0;
		float max_carry_weight = 0;
		float carry_weight = 0;
		float total_weight = 0;
		float w_spd = 0;
		float f_spd = 0;
		float less_act_time = 0;
	};*/

	//Calculate the derived stats
	//SetMaxHealth (based on class and possibly size; also sets 30/20/10% thresholds)
	//SetHealth
	//SetDodge (also sets defs)
	//SetNatArmor (also sets defs)
	//SetWornArmor (also sets defs)
	//SetAbilityScore (also sets saves and ancillary stats, including dodge and self_weight)
	
}

void Creature::SetAbilityScore(Ab_Scores a_s, float new_score) {
	if (new_score < 0) {
		cout << "Ability scores cannot be <0!" << endl;
		new_score = 0;
	}
	switch (a_s) {
		//Derived stats: fort, self_weight, max_carry_weight, and possibly f_spd and m_def
		case Ab_Scores::STR:
			stats.str = new_score;
			stats.fort = (stats.str+stats.con)*.5;
			switch (stats.size) {
				case Sizes::TINY:
					stats.self_weight = 1 + (stats.str);
					stats.max_carry_weight = max(3.f, stats.str * 10);
				break;

				case Sizes::SMALL:
					stats.self_weight = 75 + (stats.str*10);
					stats.max_carry_weight = max(12.5f, stats.str * 50);
				break;

				case Sizes::MED:
					stats.self_weight = 150 + (stats.str*10);
					stats.max_carry_weight = max(15.f, stats.str * 50);
				break;

				case Sizes::BIG:
					stats.self_weight = 190 + (stats.str*10);
					stats.max_carry_weight = max(17.5f, stats.str * 50);
				break;

				case Sizes::LARGE:
					stats.self_weight = 400 + (stats.str*25);
					stats.max_carry_weight = max(25.f, stats.str * 100);
				break;

				case Sizes::HUGE:
					stats.self_weight = 1000 + (stats.str * 100);
					stats.max_carry_weight = max(50.f, stats.str * 150);
				break;

				case Sizes::MASSIVE:
					stats.self_weight = 2500 + (stats.str * 250);
					stats.max_carry_weight = max(75.f, stats.str * 200);
				break;

				case Sizes::COLOSSAL:
					stats.self_weight = 10000 + (stats.str * 1000);
					stats.max_carry_weight = max(100.f, stats.str * 250);
				break;
			}
			if (winged) {

			}
		break;

		//Derived stats: fort, max_hlth, th_per_hlth, tw_per_hlth, tn_per_health
		case Ab_Scores::CON:
			stats.con = new_score;
			stats.fort = (stats.str + stats.con) * .5;
			//TO-DO
			switch (stats.clss) {
				case Classes::NONE:
					//TO-DO
				break;

				case Classes::ARCANIST:

				break;

				case Classes::ROGUE:

				break;

				case Classes::WARRIOR:

				break;
			}
		break;

		//Derived stats: 
		case Ab_Scores::AGI:

		break;

		case Ab_Scores::DEX:

		break;

		case Ab_Scores::INT:

		break;

		case Ab_Scores::WIS:

		break;

		case Ab_Scores::CHA:

		break;
	}
}
