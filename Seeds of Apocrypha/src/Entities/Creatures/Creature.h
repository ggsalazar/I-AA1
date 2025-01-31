#pragma once
#include "../Entity.h"

class Item;

struct Stats {
	string name = "Default";
	Sizes size = Sizes::TINY;
	Classes clss = Classes::NONE;
	//Ability scores, or primary stats
	float str = 0;
	float con = 0;
	float dex = 0;
	float agi = 0;
	float intl = 0;
	float wis = 0;
	float cha = 0;
	//Derived stats
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
	float w_spd = 0;
	float f_spd = 0;
	float self_weight = 0;
	float max_carry_weight = 0;
	float carry_weight = 0;
	float total_weight = 0;
	float less_act_time = 0;
};

class Creature : public Entity {
public:
	bool w_rsted = true;

	Creature(const Structure& s, const AnimInfo& a_i, const Animation::Transform& t = {}, const Stats& init_stats,
		const bool init_biped = true, const bool init_winged = false, const int init_dfc = 0);

	void SetAbilityScore(Ab_Scores a_s, float new_score);
	float GetAbilityScore(Ab_Scores a_s);

	void SetMaxHealth(float new_max);
	float GetMaxHealth() const { return stats.max_hlth; }

	void SetHealth(float new_hlth);
	float GetHealth() const { return stats.hlth; }

	sf::Vector2f GetDEF() const { return { stats.m_def, stats.r_def }; }
	sf::Vector3f GetSaves() const { return { stats.fort, stats.ref, stats.will }; }

protected:
	Stats stats;
	bool biped = true;
	bool winged = false;
	float action_carryover = 0;

	unordered_map<Items, unique_ptr<Item>> inv;
	unordered_map<Items, unique_ptr<Item>> equipment;

	void SetDEF();
};