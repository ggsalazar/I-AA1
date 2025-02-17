#include "Creature.h"

Creature::Creature(const Structure& s, const AnimInfo& a_i, const Animation::Transform& t, const Stats& init_stats,
	const string por_name, const bool init_biped, const bool init_winged, const int init_dfc) :
	Entity(s, a_i, t, init_dfc), 
	stats(init_stats), biped(init_biped), winged(init_winged), 
	portrait_tex("assets/Sprites/"+por_name+".png"),
	portrait(portrait_tex) {

	portrait.setOrigin(sf::Vector2f(portrait_tex.getSize()) * .5f);
	portrait.setScale({ t.scale.x * 3, t.scale.y * 3 });

	//Set base_spd
	switch (stats.size) {
		case Sizes::TINY:
			stats.base_spd = .5;
		break;

		case Sizes::SMALL:
			stats.base_spd = 1.5;
			dodge_penalty = -2;
		break;
		
		case Sizes::MED:
			stats.base_spd = 2;
			dodge_penalty = -3;
		break;
		
		case Sizes::BIG:
			stats.base_spd = 2.5;
			dodge_penalty = -4;
		break;

		case Sizes::LARGE:
			stats.base_spd = 3;
			dodge_penalty = -5;
		break;

		case Sizes::HUGE:
			stats.base_spd = 3.5;
			dodge_penalty = -6;
		break;
			
		case Sizes::MASSIVE:
			stats.base_spd = 4;
			dodge_penalty = -7;
		break;

		case Sizes::COLOSSAL:
			stats.base_spd = 5;
			dodge_penalty = -8;
		break;
	}

	//Calculate the derived stats
	//SetAbilityScore (also sets saves and ancillary stats)
	SetAbilityScore(Ab_Scores::STR, stats.str);
	SetAbilityScore(Ab_Scores::CON, stats.con);
	SetAbilityScore(Ab_Scores::DEX, stats.dex);
	SetAbilityScore(Ab_Scores::AGI, stats.agi);
	SetAbilityScore(Ab_Scores::INT, stats.intl);
	SetAbilityScore(Ab_Scores::WIS, stats.wis);
	SetAbilityScore(Ab_Scores::CHA, stats.cha);
	//SetMaxHealth (based on size, genus, level, and class; also sets 30/20/10% thresholds)
	SetMaxHealth();
	//SetHealth
	SetHealth(stats.max_hlth);
	SetDEF();

	//Natural and worn armor will have to be set manually
}

void Creature::Draw(const bool debug) {
	Entity::Draw(debug);
	//Set the camera to the hud
	structure.window.setView(structure.game.hud);

	//Creatures will draw their portraits in combat at the proper location and sizing (currently acting combatant's frame will be slightly larger and have a special frame)


	//Set the camera back to the world
	structure.window.setView(structure.game.camera);
}

void Creature::SetAbilityScore(Ab_Scores a_s, float new_score) {
	if (new_score < 0) {
		cout << "Ability scores cannot be <0!" << endl;
		new_score = 0;
	}
	switch (a_s) {
		//Derived stats: fort, self_weight, max_carry_weight, f_spd, and possibly m_def
		case Ab_Scores::STR:
			stats.str = new_score;
			stats.fort = (stats.str+stats.con)*.5f;
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
			//Determine flying speed
			SetFlySpeed();
		break;

		//Derived stats: fort, max_hlth, th_per_hlth, tw_per_hlth, tn_per_health
		//Setting Health is a special case, and is handled in SetMaxHealth()
		case Ab_Scores::CON:
			stats.con = new_score;
			stats.fort = (stats.str + stats.con) * .5f;
		break;

		//Derived stats: Ref, w_spd, dodge
		case Ab_Scores::AGI:
			stats.agi = new_score;
			stats.ref = (stats.agi + stats.dex) * .5f;
			stats.w_spd = stats.base_spd + (.5f * stats.agi);
			stats.dodge = max(0.f, stats.dex + stats.agi + dodge_penalty);
		break;

		//Derived stats: Ref, less_action_time, dodge
		case Ab_Scores::DEX:
			stats.dex = new_score;
			stats.ref = (stats.agi + stats.dex) * .5f;
			stats.dodge = max(0.f, stats.dex + stats.agi + dodge_penalty);
		break;

		//No derived stats
		case Ab_Scores::INT:
			stats.intl = new_score;
		break;

		//Derived stats: Will
		case Ab_Scores::WIS:
			stats.wis = new_score;
			stats.will = (stats.wis + stats.cha) * .5f;
		break;

		//Derived stats: Will
		case Ab_Scores::CHA:
			stats.cha = new_score;
			stats.will = (stats.wis + stats.cha) * .5f;
		break;
	}
}

float Creature::GetAbilityScore(Ab_Scores a_s) {
	switch (a_s) {
		case Ab_Scores::STR:
			return stats.str;
		break;

		case Ab_Scores::CON:
			return stats.con;
		break;

		case Ab_Scores::AGI:
			return stats.agi;
		break;

		case Ab_Scores::DEX:
			return stats.dex;
		break;

		case Ab_Scores::INT:
			return stats.intl;
		break;

		case Ab_Scores::WIS:
			return stats.wis;
		break;

		case Ab_Scores::CHA:
			return stats.cha;
		break;
	}

	cout << "You done goofed" << endl;
	return 0.0f;
}

void Creature::SetMaxHealth() {
	float old_max = stats.max_hlth;
	//For sentients, max health depends on class and level
	if (stats.genus == Genuses::SENTIENT) {
		switch (stats.clss) {
			case Classes::NONE:
				stats.max_hlth = 5;
			break;

			case Classes::ARCANIST:
				stats.max_hlth = (stats.level * 6) + (stats.con * stats.level);
			break;

			case Classes::ROGUE:
				stats.max_hlth = (stats.level * 8) + (stats.con * stats.level);
			break;

			case Classes::WARRIOR:
				stats.max_hlth = (stats.level * 10) + (stats.con * stats.level);
			break;
		}
	}

	//For non-sentients, max health partially depends on size but is mostly arbitrary
	else {}

	//Set the 30/20/10% health threshholds
	stats.th_per_hlth = stats.max_hlth * .3f;
	stats.tw_per_hlth = stats.max_hlth * .2f;
	stats.tn_per_hlth = stats.max_hlth * .1f;

	//Set current health
	stats.hlth += stats.max_hlth - old_max;
}

void Creature::SetHealth(float new_hlth) {
	if (new_hlth > stats.max_hlth) new_hlth = stats.max_hlth;
	stats.hlth = max(0.f, new_hlth);
}

void Creature::SetNatArmor(float n_a) {
	n_a = max(0.f, n_a);
	stats.nat_armor = n_a;
	stats.armor = stats.nat_armor + stats.worn_armor;
	SetDEF();
}

void Creature::SetWornArmor(float w_a) {
	w_a = max(0.f, w_a);
	stats.worn_armor = w_a;
	stats.armor = stats.nat_armor + stats.worn_armor;
	SetDEF();
}

void Creature::SetDEF() {
	stats.m_def = stats.dodge + stats.armor;

	switch (stats.size) {
		case Sizes::TINY:
			stats.r_def = stats.m_def + 6;
		break;

		case Sizes::SMALL:
			stats.r_def = stats.m_def + 4;
		break;

		case Sizes::MED:
			stats.r_def = stats.m_def + 3;
		break;

		case Sizes::BIG:
			stats.r_def = stats.m_def + 2;
		break;

		case Sizes::LARGE:
		case Sizes::HUGE:
		case Sizes::MASSIVE:
		case Sizes::COLOSSAL:
			stats.r_def = stats.m_def + 1;
		break;
	}

}
