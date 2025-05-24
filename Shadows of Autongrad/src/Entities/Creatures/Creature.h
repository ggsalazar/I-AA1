#pragma once
#include <queue>
#include "../Entity.h"
#include "../../Core/Math.h"
#include "../../Core/Input.h" //Window
#include "../../Core/Vector3.h"

class Creature : public Entity {
public:
	struct Stats {
		//High-level details
		string name = "Default";
		Genus genus = Genus::Sentient;
		Race race = Race::Human;
		Size size = Size::Med;
		Class clss = Class::None;
		uint level = 0;
		bool sex = 0; //0 = F, 1 = M
		//Ability scores, or primary stats
		float str = 0;
		float con = 0;
		float agi = 0;
		float dex = 0;
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
		float nat_armor = 1;
		float worn_armor = 0;
		float m_def = 0;
		float r_def = 0;
		float fort = 0;
		float ref = 0;
		float will = 0;
		float w_spd = 0;
		float f_spd = 0;
		//To add: Swim speed!
		float max_carry_weight = 0;
		float carry_weight = 0;
		float total_weight = 0;
		float less_act_time = 0;
	};
	bool well_rested = true;

	bool moving = false;

	Sprite portrait;
	Rect por_bbox;

	Creature(const Sprite::Info& s_i, const Stats& init_stats = {},
		const string por_name = "Placeholder", const int init_dispo = 50,
		const bool init_biped = true, const bool init_winged = false);

	//Engine stuff
	virtual void GetInput() override;
	virtual void Update() override;
	virtual void Draw() override;

	//Path stuff
	inline void SetPath(queue<Vector2i> new_path) { path = new_path; }
	void WalkPath();

	//Printing stats
	void PrintStats();

	//Primary stats
	void SetAbilityScores(array<float, 7> a_ss);
	inline array<float, 7> GetAbilityScores() const { return { stats.str, stats.con, stats.agi, stats.dex, stats.intl, stats.wis, stats.cha }; }
	void SetAbilityScore(Ab_Score a_s, float new_score);
	float GetAbilityScore(Ab_Score a_s);

	//Derived stats
	void SetMaxHealth();
	inline float GetMaxHealth() const { return stats.max_hlth; }

	void SetHealth(float new_hlth);
	inline float GetHealth() const { return stats.hlth; }

	void SetNatArmor(float n_a);
	inline uint GetNatArmor() const { return stats.nat_armor; }

	void SetWornArmor(float w_a);
	inline float GetWornArmor() const { return stats.worn_armor; }

	inline void SetFlySpeed() { if (can_fly) stats.f_spd = base_spd + (.5f * stats.str); }
	inline float GetFlySpeed() const { return stats.f_spd; }


	//Functions that interact with protected members
	inline string GetName() const { return stats.name; }
	inline Vector2f GetDEF() const { return { stats.m_def, stats.r_def }; }
	inline Vector3f GetSaves() const { return { stats.fort, stats.ref, stats.will }; }
	inline int GetDispo() const { return party_dispo; }


	inline void SetCanFly(bool c_f = true) { can_fly = c_f; SetFlySpeed(); }
	void SetDispo(int new_dispo);

	void AlterDispo(int offset);

	void PrintDispo();

protected:
	Stats stats;
	bool biped = true;
	bool winged = false;
	bool can_fly = false;
	float base_spd = 0;
	float self_weight = 0;
	float action_carryover = 0;
	int dodge_penalty = 0;
	bool encumbered = false;
	bool in_combat = false;
	int party_dispo = 50;
	float mv_spd = 1; //This is not a stat, this is the UTH movement speed!


	Text nameplate;

	//unordered_map<Items, Item> inv;
	//unordered_map<Items, Item> equipment;

	queue<Vector2i> path;

	void SetDEF();
};