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
		bool sex = 0; //0 = F, 1 = M
		//Levels - Arcanist, Rogue, Warrior, Other
		array<uint, 4> levels = {0};
		//Ability scores - SCADIWC
		array<float, 7> a_scores = { 0 };
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
	
	struct Skills {
		//Action Skills
			//STR
		uint climb = 1, climb_max = 5,
			swim = 1, swim_max = 5,
			//DEX
			soh = 1, soh_max = 5,//Sleight of Hand
			stealth = 1, stealth_max = 5,
			//INT
			arcana = 1, arcana_max = 5,
			lore = 1, lore_max = 5,
			mechanics = 1, mechanics_max = 5,
			medicine = 1, medicine_max = 5,
			nature = 1, nature_max = 5,
			//WIS
			appraisal = 1, appraisal_max = 5,
			beast_handling = 1, beast_handling_max = 5,
			insight = 1, insight_max = 5,
			navigation = 1, navigation_max = 5,
			perception = 1, perception_max = 5,
			survival = 1, survival_max = 5,
			//CHA
			beguilement = 1, beguilement_max = 5,
			intimidation = 1, intimidation_max = 5,
			persuasion = 1, persuasion_max = 5,
			//Tool Skills
			thieving_tools = 0, thieving_tools_max = 5,
			medicine_kits = 0, medicine_kits_max = 5,
			//Weapon Skills
			//WHAT TO CALL ARCANIST SPECIAL WEAPON SKILL????? ARCANE????
			archery = 0, archery_max = 5,
			exotic = 0, exotic_max = 5,
			firearm = 0, firearm_max = 5,
			large = 0, large_max = 5,
			medium = 0, medium_max = 5,
			polearm = 0, polearm_max = 5,
			small = 0, small_max = 5,
			throwing = 0, throwing_max = 5,
			unarmed = 0, unarmed_max = 5;
	};

	struct Inventory {
		float aeons = 10;
		//unordered_map<Items, Item> equipment;
		//unordered_map<Items, Item> equipment;
	};

	bool well_rested = true;
	bool alive = true;

	//string dlg_node = "DEFAULT";
	string dlg_node = "Test";

	Sprite portrait;
	Rect por_bbox;

	Creature(const Sprite::Info& s_i, const Stats& init_stats = {},
		const string por_name = "Placeholder", const int init_dispo = 50,
		const bool init_biped = true, const bool init_winged = false,
		const string init_dlg_node = "Test");

	//Engine stuff
	virtual void GetInput() override;
	virtual void Update() override;
	virtual void Draw() override;

	//Path stuff
	inline void SetPath(queue<Vector2i> new_path) { path = new_path; }
	inline queue<Vector2i> GetPath() const { return path; }
	void WalkPath();

	//Printing info
	void PrintStats();
	void PrintSkills();
	void PrintInv();
	void PrintDispo();


	//High-level info
	inline string GetName() const { return stats.name; }
	inline array<uint, 4> GetLevels() const { return stats.levels; }

	//Primary stats
	void SetAbilityScores(array<float, 7> a_ss);
	inline array<float, 7> GetAbilityScores() const { return stats.a_scores; }
	void SetAbilityScore(Ab_Score a_s, float new_score);
	float GetAbilityScore(Ab_Score a_s);

	//Derived stats
	void SetMaxHealth();
	inline float GetMaxHealth() const { return stats.max_hlth; }

	void SetHealth(float new_hlth);
	inline float GetHealth() const { return stats.hlth; }

	//Saves set in SetAbilityScore()
	inline Vector3f GetSaves() const { return { stats.fort, stats.ref, stats.will }; }

	void SetNatArmor(float n_a);
	inline uint GetNatArmor() const { return stats.nat_armor; }

	void SetWornArmor(float w_a);
	inline float GetWornArmor() const { return stats.worn_armor; }
	inline Vector2f GetDEF() const { return { stats.m_def, stats.r_def }; }

	inline void SetFlySpeed() { if (can_fly) stats.f_spd = base_spd + (.5f * stats.a_scores[0]); }
	inline void SetCanFly(bool c_f = true) { can_fly = c_f; SetFlySpeed(); }
	inline float GetFlySpeed() const { return stats.f_spd; }
	
	//Skills


	//Inventory
	void SetAeons(float new_aeons);
	void AlterAeons(float offset);
	inline float GetAeons() const { return inv.aeons; }

	void SetDispo(float new_dispo);
	void AlterDispo(float offset);
	inline float GetDispo() const { return party_dispo; }

protected:
	Stats stats;
	Skills skills;
	Inventory inv;

	bool biped = true;
	bool winged = false;
	bool can_fly = false;
	float base_spd = 0;
	float self_weight = 0;
	float action_carryover = 0;
	int dodge_penalty = 0;
	bool encumbered = false;
	bool in_combat = false;
	float party_dispo = 50;
	float base_mv_spd = 1;
	float mv_spd = 1; //This is not a stat, this is the UTH movement speed!


	Text nameplate;

	queue<Vector2i> path;

	void SetDEF();
};