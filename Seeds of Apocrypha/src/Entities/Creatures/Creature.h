#pragma once
#include "../Entity.h"
#include "../Items/Item.h"



class Creature : public Entity {
public:
	struct Stats {
		string name = "Default";
		Genuses genus = Genuses::SENTIENT;
		Races race = Races::HUMAN;
		Sizes size = Sizes::MED;
		Classes clss = Classes::NONE;
		uint level = 0;
		bool sex = 0; //0 = F, 1 = M
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
		float armor = 0;
		float nat_armor = 0;
		float worn_armor = 0;
		float m_def = 0;
		float r_def = 0;
		float fort = 0;
		float ref = 0;
		float will = 0;
		float base_spd = 0;
		float w_spd = 0;
		float f_spd = 0;
		float self_weight = 0;
		float max_carry_weight = 0;
		float carry_weight = 0;
		float total_weight = 0;
		float less_act_time = 0;
	};
	bool w_rsted = true;

	sf::Texture portrait_tex;
	sf::Sprite portrait;
	sf::FloatRect por_bbox;

	Creature(const Engine& e, const AnimInfo& a_i, const Animation::Transform& t = {}, const Stats& init_stats = {}, 
		const string por_name = "Creatures/Portraits/Placeholder", const bool init_biped = true, const bool init_winged = false, const int init_dfc = 0);

	//Game stuff
	virtual void Update() override;
	virtual void Draw() override;

	//Primary stats
	void SetAbilityScore(Ab_Scores a_s, float new_score);
	float GetAbilityScore(Ab_Scores a_s);

	//Derived stats
	void SetMaxHealth();
	float GetMaxHealth() const { return stats.max_hlth; }

	void SetHealth(float new_hlth);
	float GetHealth() const { return stats.hlth; }

	void SetNatArmor(float n_a);
	uint GetNatArmor() const { return stats.nat_armor; }

	void SetWornArmor(float w_a);
	float GetWornArmor() const { return stats.worn_armor; }

	void SetFlySpeed() { if (winged and can_fly) stats.f_spd = stats.base_spd + (.5f * stats.str); }
	float GetFlySpeed() const { return stats.f_spd; }

	//Functions that interact with protected members
	string GetName() const { return stats.name; }
	sf::Vector2f GetDEF() const { return { stats.m_def, stats.r_def }; }
	sf::Vector3f GetSaves() const { return { stats.fort, stats.ref, stats.will }; }

	void SetCanFly(bool c_f = true) { can_fly = c_f; SetFlySpeed(); }

protected:
	Stats stats;
	bool biped = true;
	bool winged = false;
	bool can_fly = false;
	float action_carryover = 0;
	int dodge_penalty = 0;
	bool encumbered = false;
	bool in_combat = false;
	sf::Text nameplate;

	unordered_map<Items, unique_ptr<Item>> inv;
	unordered_map<Items, unique_ptr<Item>> equipment;


	void SetDEF();
};