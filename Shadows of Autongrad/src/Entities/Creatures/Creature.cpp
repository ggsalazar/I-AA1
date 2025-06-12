#include "Creature.h"

Creature::Creature(const Sprite::Info& s_i, const Stats& init_stats,
	const string por_name, const int init_dispo,
	const bool init_biped, const bool init_winged,
	const string init_dlg_branch)
	: Entity(s_i), stats(init_stats), party_dispo(init_dispo), biped(init_biped), winged(init_winged),
		dlg_branch(init_dlg_branch) {

	//Set sprite origin (most likely {.5f, .95f} for all creatures)
	sprite.SetOrigin({ .5f, .95f });
	sprite.SetScale(game->GetResScale());
	Entity::MoveTo(float_pos);

	//Portrait + portrait bbox
	Sprite::Info por_info = {};
	por_info.sheet = "Creatures/Portraits/" + por_name; por_info.frame_size = { 48, 48 };
	por_info.scale = game->GetResScale();
	portrait.Init(por_info);
	//Portrait bbox
	por_bbox.w = portrait.GetSprSize().x * 1.05f;
	por_bbox.h = portrait.GetSprSize().y * 1.05f;

	//Nameplate
	Text::Info np_info = {};
	np_info.str = stats.name; np_info.max_width = por_bbox.w;
	nameplate.Init(&game->default_fonts[36], np_info);

	//Set base_spd and dodge_penalty
	switch (stats.size) {
	case Size::Tiny:
		base_spd = .5;
		break;

	case Size::Small:
		base_spd = 1.5;
		dodge_penalty = -2;
		break;

	case Size::Med:
		base_spd = 2;
		dodge_penalty = -3;
		break;

	case Size::Big:
		base_spd = 2.5;
		dodge_penalty = -4;
		break;

	case Size::Large:
		base_spd = 3;
		dodge_penalty = -5;
		break;

	case Size::Huge:
		base_spd = 3.5;
		dodge_penalty = -6;
		break;

	case Size::Massive:
		base_spd = 4;
		dodge_penalty = -7;
		break;

	case Size::Colossal:
		base_spd = 5;
		dodge_penalty = -8;
		break;
	}
	//Set UTH move speed
	base_mv_spd = 400.f;
	//Factor in whether or not the creature is bipedal (if not, they get 2 extra m/s)
	base_spd += !biped * 2;

	//Calculate the derived stats
	//Set Ability Scores (also sets saves and ancillary stats)
	SetAbilityScores(stats.a_scores);
	//Set Health and defenses (based on size, genus, level, and class; also sets 30/20/10% thresholds)
	SetMaxHealth();
	SetHealth(stats.max_hlth);
	SetDEF();

	//Natural and worn armor will have to be set manually


	stats.total_weight = self_weight + stats.carry_weight;
	encumbered = stats.carry_weight > stats.max_carry_weight;
	mv_spd = encumbered ? base_mv_spd * .5f : base_mv_spd;


	//PrintStats();
}

void Creature::GetInput() {
	if (Collision::RectPoint(por_bbox, Input::MousePos()))
		scene->lmb_action = false;
}

void Creature::Update() {
	Entity::Update();

	//I probably don't need to be setting this every frame, but this works for now
	stats.total_weight = self_weight + stats.carry_weight;
	encumbered = stats.carry_weight > stats.max_carry_weight;
	mv_spd = encumbered ? base_mv_spd * .5f : base_mv_spd;

	//Position of portrait in combat will be updated here

	por_bbox.x = portrait.GetPos().x - round(portrait.GetSprSize().x * .025f);
	por_bbox.y = portrait.GetPos().y - round(portrait.GetSprSize().y * .025f);

	//Moving
	if (!path.empty() and !game->paused) WalkPath();
}

void Creature::Draw() {
	Entity::Draw();

	//Creatures will draw their portraits in combat at the proper location and sizing
	// currently acting combatant's frame will be slightly larger and have a special frame

	game->renderer.DrawPath(path);
}

void Creature::WalkPath() {
	if (!path.empty()) {
		Vector2i next_pos = path.front();

		if (pos != next_pos) {

			Vector2f offset = { 0 };

			//Moving up
			if (pos.y > next_pos.y)
				offset.y = -mv_spd;
			//Moving down
			else if (pos.y < next_pos.y)
				offset.y = mv_spd;
			//Moving left
			if (pos.x > next_pos.x)
				offset.x = -mv_spd;
			//Moving right
			else if (pos.x < next_pos.x)
				offset.x = mv_spd;

			//Normalize diagonal movement
			if (offset.x != 0 and offset.y != 0)
				offset /= sqrt2;

			offset *= game->delta_time;

			Entity::MoveBy(offset);

			if (abs(pos.x - next_pos.x) < 5) Entity::MoveTo(Vector2f(next_pos.x, pos.y));
			if (abs(pos.y - next_pos.y) < 5) Entity::MoveTo(Vector2f(pos.x, next_pos.y));

		}
		else
			path.pop();
	}
}

void Creature::PrintStats() {
	cout << "Name: " << stats.name << "\n";
	cout << "Genus: " << GenusToString(stats.genus) << "\n";
	cout << "Race: " << RaceToString(stats.race) << "\n";
	cout << "Size: " << SizeToString(stats.size) << "\n";
	cout << "Sex: ";
	if (stats.sex) cout << "M\n";
	else cout << "F\n";
	cout << "Levels: ";
	cout << "Arcanist: " << stats.levels[0] << "; Rogue: " << stats.levels[1] << "; Warrior: " << stats.levels[2] << "; Other: " << stats.levels[3] << "\n";
	cout << "STR: " << stats.a_scores[0] << ", CON: " << stats.a_scores[1] << ", AGI: " << stats.a_scores[2] << ", DEX: " << stats.a_scores[3] << ", INT: " << stats.a_scores[4] << ", WIS: " << stats.a_scores[5] << ", CHA: " << stats.a_scores[6] << "\n";
	cout << "Health: " << stats.hlth << "/" << stats.max_hlth << "\n";
	cout << "Health Threshholds: 30%: " << stats.th_per_hlth << "; 20%: " << stats.tw_per_hlth << "; 10%: " << stats.tn_per_hlth << "\n";
	cout << "Defenses:\nM-DEF: " << stats.m_def << " (Dodge: " << stats.dodge << " (AGI: " << stats.a_scores[2] << " + DEX: " << stats.a_scores[3] << " + Dodge Penalty: " << dodge_penalty << "), Armor: " << stats.armor << " (Nat: " << stats.nat_armor << ", Worn: " << stats.worn_armor << ")); R-DEF: " << stats.r_def << "\n";
	cout << "Saves:\nFort: " << stats.fort << "; Ref: " << stats.ref << "; Will: " << stats.will << "\n";
	cout << "Movement:\nWalking Speed: " << stats.w_spd << "m/s (" << base_spd << " + " << stats.a_scores[2]*.5 <<"); Flying Speed: " << stats.f_spd << " (" << base_spd << " + " << stats.a_scores[0] * .5 << ")\n"; //To add: Swim Speed!
	cout << "Weight:\nTotal Weight: " << stats.total_weight << "lbs. (Self Weight: " << self_weight << ", Carry Weight: " << stats.carry_weight << "/" << stats.max_carry_weight;
	if (encumbered) cout << "(Encumbered!)";
	cout << ")\n";
	cout << "Other:\nAction Time Reduction: " << stats.less_act_time << "s; ";
	if (biped) cout << "Biped; ";
	else cout << "Quadruped; ";
	if (winged) cout << "Winged; ";
	if (can_fly) cout << "Can ";
	else cout << "Cannot ";
	cout << "Fly; ";
	if (!well_rested) cout << "Not ";
	cout << "Well-rested\n";
	PrintDispo();
}

void Creature::PrintSkills() {
	cout << "Action Skills:\n";
	cout << "Arcana: " << skills.arcana << "/" << skills.arcana_max << "\n";
	cout << "Appraisal: " << skills.appraisal << "/" << skills.appraisal_max << "\n";
	cout << "Beast Handling: " << skills.beast_handling << "/" << skills.beast_handling_max << "\n";
	cout << "Beguilement: " << skills.beguilement << "/" << skills.beguilement_max << "\n";
	cout << "Climb: " << skills.climb << "/" << skills.climb_max << "\n";
	cout << "Insight: " << skills.insight << "/" << skills.insight_max << "\n";
	cout << "Intimidation: " << skills.intimidation << "/" << skills.intimidation_max << "\n";
	cout << "Lore: " << skills.lore << "/" << skills.lore_max << "\n";
	cout << "Mechanics: " << skills.mechanics << "/" << skills.mechanics_max << "\n";
	cout << "Medicine: " << skills.medicine << "/" << skills.medicine_max << "\n";
	cout << "Nature: " << skills.nature << "/" << skills.nature_max << "\n";
	cout << "Navigation: " << skills.navigation << "/" << skills.navigation_max << "\n";
	cout << "Perception: " << skills.perception << "/" << skills.perception_max << "\n";
	cout << "Persuasion: " << skills.persuasion << "/" << skills.persuasion_max << "\n";
	cout << "Sleight of Hand: " << skills.soh << "/" << skills.soh_max << "\n";
	cout << "Stealth: " << skills.stealth << "/" << skills.stealth_max << "\n";
	cout << "Survival: " << skills.survival << "/" << skills.survival_max << "\n";
	cout << "Swim: " << skills.swim << "/" << skills.swim_max << "\n";
	
	cout << "Weapon Skills:\n";
	cout << "Archery: " << skills.archery << "/" << skills.archery_max << "\n";
	cout << "Exotic: " << skills.exotic << "/" << skills.exotic_max << "\n";
	cout << "Firearm: " << skills.firearm << "/" << skills.firearm_max << "\n";
	cout << "Large: " << skills.large << "/" << skills.large_max << "\n";
	cout << "Medium: " << skills.medium << "/" << skills.medium_max << "\n";
	cout << "Polearm: " << skills.polearm << "/" << skills.polearm_max << "\n";
	cout << "Small: " << skills.small << "/" << skills.small_max << "\n";
	cout << "Throwing: " << skills.throwing << "/" << skills.throwing_max << "\n";
	cout << "Unarmed: " << skills.unarmed << "/" << skills.unarmed_max << "\n";

	cout << "Tool Skills:\n";
	cout << "Thieving Tools: " << skills.thieving_tools << "/" << skills.thieving_tools_max << "\n";
	cout << "Medicine Kits: " << skills.medicine_kits << "/" << skills.medicine_kits_max << "\n";
}

void Creature::PrintInv() {
	cout << "Inventory:\n";
}

void Creature::PrintDispo() {
	cout << "Party Disposition: " << party_dispo;
	if (0 == party_dispo) cout << " (True Enemy)\n";
	else if (0 < party_dispo and party_dispo <= 20) cout << " (Hostile)\n";
	else if (20 < party_dispo and party_dispo <= 40) cout << " (Testy)\n";
	else if (40 < party_dispo and party_dispo < 60) cout << " (Neutral)\n";
	else if (60 <= party_dispo and party_dispo < 80) cout << " (Amicable)\n";
	else if (80 <= party_dispo and party_dispo < 100) cout << " (Friendly)\n";
	else if (party_dispo == 100) cout << " (True Friend)\n";
	else cout << "!!! Invalid Disposition!!!\n";
}

void Creature::SetAbilityScores(array<float, 7> a_ss) {
	SetAbilityScore(Ab_Score::STR, a_ss[0]);
	SetAbilityScore(Ab_Score::CON, a_ss[1]);
	SetAbilityScore(Ab_Score::AGI, a_ss[2]);
	SetAbilityScore(Ab_Score::DEX, a_ss[3]);
	SetAbilityScore(Ab_Score::INT, a_ss[4]);
	SetAbilityScore(Ab_Score::WIS, a_ss[5]);
	SetAbilityScore(Ab_Score::CHA, a_ss[6]);
}

void Creature::SetAbilityScore(Ab_Score a_s, float new_score) {
	if (new_score < 0) {
		cout << "Ability scores cannot be <0!\n";
		new_score = 0;
	}
	switch (a_s) {
		//Derived stats: fort, self_weight, max_carry_weight, f_spd, and possibly m_def
		case Ab_Score::STR:
			stats.a_scores[0] = new_score;
			stats.fort = (stats.a_scores[0] + stats.a_scores[1]) * .5f;
			switch (stats.size) {
			case Size::Tiny:
				self_weight = 1 + (stats.a_scores[0]);
				stats.max_carry_weight = max(3.f, stats.a_scores[0] * 10);
				break;

			case Size::Small:
				self_weight = 75 + (stats.a_scores[0] * 10);
				stats.max_carry_weight = max(12.5f, stats.a_scores[0] * 50);
				break;

			case Size::Med:
				self_weight = 150 + (stats.a_scores[0] * 10);
				stats.max_carry_weight = max(15.f, stats.a_scores[0] * 50);
				break;

			case Size::Big:
				self_weight = 190 + (stats.a_scores[0] * 10);
				stats.max_carry_weight = max(17.5f, stats.a_scores[0] * 50);
				break;

			case Size::Large:
				self_weight = 400 + (stats.a_scores[0] * 25);
				stats.max_carry_weight = max(25.f, stats.a_scores[0] * 100);
				break;

			case Size::Huge:
				self_weight = 1000 + (stats.a_scores[0] * 100);
				stats.max_carry_weight = max(50.f, stats.a_scores[0] * 150);
				break;

			case Size::Massive:
				self_weight = 2500 + (stats.a_scores[0] * 250);
				stats.max_carry_weight = max(75.f, stats.a_scores[0] * 200);
				break;

			case Size::Colossal:
				self_weight = 10000 + (stats.a_scores[0] * 1000);
				stats.max_carry_weight = max(100.f, stats.a_scores[0] * 250);
				break;
			}
			//Determine flying speed
			SetFlySpeed();
		break;

		//Derived stats: fort, max_hlth, th_per_hlth, tw_per_hlth, tn_per_health
		//Setting Health is a special case, and is handled in SetMaxHealth()
		case Ab_Score::CON:
			stats.a_scores[1] = new_score;
			stats.fort = (stats.a_scores[0] + stats.a_scores[1]) * .5f;
		break;

		//Derived stats: Ref, w_spd, dodge
		case Ab_Score::AGI:
			stats.a_scores[2] = new_score;
			stats.ref = (stats.a_scores[2] + stats.a_scores[3]) * .5f;
			stats.w_spd = base_spd + (.5f * stats.a_scores[2]);
			stats.dodge = max(0.f, stats.a_scores[3] + stats.a_scores[2] + dodge_penalty); //Remember this is modified by armor!
		break;

		//Derived stats: Ref, less_action_time, dodge
		case Ab_Score::DEX:
			stats.a_scores[3] = new_score;
			stats.ref = (stats.a_scores[2] + stats.a_scores[3]) * .5f;
			stats.less_act_time = stats.a_scores[3] * .25f; //Need to round this to the nearest half (or quarter?)
			stats.dodge = max(0.f, stats.a_scores[3] + stats.a_scores[2] + dodge_penalty); //Remember this is modified by armor!
		break;

		//No derived stats, but can get more skill points/languages
		case Ab_Score::INT:
			stats.a_scores[4] = new_score;
		break;

		//Derived stats: Will
		case Ab_Score::WIS:
			stats.a_scores[5] = new_score;
			stats.will = (stats.a_scores[5] + stats.a_scores[6]) * .5f;
		break;

		//Derived stats: Will
		case Ab_Score::CHA:
			stats.a_scores[6] = new_score;
			stats.will = (stats.a_scores[5] + stats.a_scores[6]) * .5f;
		break;
	}
}

float Creature::GetAbilityScore(Ab_Score a_s) {
	switch (a_s) {
		case Ab_Score::STR:
			return stats.a_scores[0];
			break;

		case Ab_Score::CON:
			return stats.a_scores[1];
			break;

		case Ab_Score::AGI:
			return stats.a_scores[2];
			break;

		case Ab_Score::DEX:
			return stats.a_scores[3];
			break;

		case Ab_Score::INT:
			return stats.a_scores[4];
			break;

		case Ab_Score::WIS:
			return stats.a_scores[5];
			break;

		case Ab_Score::CHA:
			return stats.a_scores[6];
			break;
	}

	cout << "You done goofed" << endl;
	return 0.f;
}

void Creature::SetMaxHealth() {
	float old_max = stats.max_hlth;
	//For sentients, max health depends on class and level
	if (stats.genus == Genus::Sentient) {
		uint total_level = 0;
		for (int l : stats.levels)
			total_level = stats.levels[l];

		//Temporary as FUCK
		if (stats.levels[0]) stats.max_hlth = (stats.levels[0] * 6) + (stats.levels[1] * 8) + (stats.levels[2] * 10) + (stats.a_scores[1] * total_level);
	}

	//For non-Sentients, max health partially depends on size but is mostly arbitrary
	else {
		switch (stats.size) {
			default:

			break;
		}
	}

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

void Creature::SetDispo(float new_dispo) {
	party_dispo = new_dispo;
	Math::Clamp(party_dispo, 0, 100);
}

void Creature::AlterDispo(float offset) {
	party_dispo += offset;
	Math::Clamp(party_dispo, 0, 100);
}

void Creature::SetDEF() {
	stats.m_def = stats.dodge + stats.armor;

	switch (stats.size) {
	case Size::Tiny:
		stats.r_def = stats.m_def + 6;
		break;

	case Size::Small:
		stats.r_def = stats.m_def + 4;
		break;

	case Size::Med:
		stats.r_def = stats.m_def + 3;
		break;

	case Size::Big:
		stats.r_def = stats.m_def + 2;
		break;

	case Size::Large:
	case Size::Huge:
	case Size::Massive:
	case Size::Colossal:
		stats.r_def = stats.m_def + 1;
		break;
	}

}