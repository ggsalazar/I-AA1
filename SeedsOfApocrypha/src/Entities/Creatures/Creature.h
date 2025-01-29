#pragma once
#include "../Entity.h"

class Creature : public Entity {
public:
	struct Stats {
		float max_hlth = 0;
		float curr_hlth = 0;
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
		float str = 0;
		float con = 0;
		float dex = 0;
		float agi = 0;
		float intel = 0;
		float wis = 0;
		float cha = 0;
		bool w_rstd = false;
		float w_spd = 0;
		float f_spd = 0;
		float carry_weight = 0;
		float total_weight = 0;
		float less_act_time = 0;
		Sizes size = Sizes::TINY;
		string name = "";
	};

	//Inventory should probably be a map...
	struct Inv {

	};

	//Equipment should also probably be a map...
	struct Equipment {

	};


	Creature(string init_name, float )

private:
	float action_carryover = 0;
};