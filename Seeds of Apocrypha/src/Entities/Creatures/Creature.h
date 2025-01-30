#pragma once
#include "../Entity.h"

class Item;

class Creature : public Entity {
public:
	struct Stats {
		string name = "";
		Sizes size = Sizes::TINY;
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
		bool w_rstd = false;
		float w_spd = 0;
		float f_spd = 0;
		float carry_weight_max = 0;
		float carry_weight = 0;
		float self_weight = 0;
		float total_weight = 0;
		float less_act_time = 0;
	};


	Creature(const Entity::Structure& s, const Animation::AnimInfo& a_i, const Animation::Transform& t = {}, const Creature::Stats& stats);

protected:
	float action_carryover = 0;

	unordered_map<Items, unique_ptr<Item>> inv;
	unordered_map<Items, unique_ptr<Item>> equipment;
};