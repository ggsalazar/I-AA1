#include "Creature.h"

Creature::Creature(const Sprite::Info& s_i, const Stats& init_stats,
	const string por_name, const bool init_biped, const bool init_winged)
	: Entity(s_i),
	stats(init_stats), biped(init_biped), winged(init_winged) {

	//Set sprite origin (most likely {.5f, .95f} for all creatures)
	sprite.SetOrigin({ .5f, .95f });
	sprite.SetScale(game->GetResScale());

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

	//Set base_spd
	switch (stats.size) {
	case Size::Tiny:
		stats.base_spd = .5;
		break;

	case Size::Small:
		stats.base_spd = 1.5;
		dodge_penalty = -2;
		break;

	case Size::Med:
		stats.base_spd = 2;
		dodge_penalty = -3;
		break;

	case Size::Big:
		stats.base_spd = 2.5;
		dodge_penalty = -4;
		break;

	case Size::Large:
		stats.base_spd = 3;
		dodge_penalty = -5;
		break;

	case Size::Huge:
		stats.base_spd = 3.5;
		dodge_penalty = -6;
		break;

	case Size::Massive:
		stats.base_spd = 4;
		dodge_penalty = -7;
		break;

	case Size::Colossal:
		stats.base_spd = 5;
		dodge_penalty = -8;
		break;
	}
	//Set move speed
	mv_spd = TS *.25f;

	//Calculate the derived stats
	//Set Ability Scores (also sets saves and ancillary stats)
	SetAbilityScores({ stats.str, stats.con, stats.agi, stats.dex, stats.intl, stats.wis, stats.cha });
	//Set Health and defenses (based on size, genus, level, and class; also sets 30/20/10% thresholds)
	SetMaxHealth();
	SetHealth(stats.max_hlth);
	SetDEF();

	//Natural and worn armor will have to be set manually
}

void Creature::GetInput() {
	if (Collision::Point(Input::MousePos(), por_bbox)) {
		scene->lmb_action = false;
	}
}

void Creature::Update() {
	//Position of portrait in combat will be updated here

	por_bbox.x = portrait.GetPos().x - round(portrait.GetSprSize().x * .025f);
	por_bbox.y = portrait.GetPos().y - round(portrait.GetSprSize().y * .025f);

	//Moving
	if (moving and !game->paused) WalkPath();
}

void Creature::Draw() {
	Entity::Draw();

	//Creatures will draw their portraits in combat at the proper location and sizing
	// currently acting combatant's frame will be slightly larger and have a special frame

	//game->renderer.DrawPath(path);
}

void Creature::WalkPath() {
	if (!path.empty()) {
		Vector2i next_pos = path.front();
		if (pos != next_pos) {

			Vector2f offset = { 0 };

			//Moving up
			if (pos.y > next_pos.y)
				offset.y = -min(mv_spd, (float)(pos.y - next_pos.y));
			//Moving down
			else if (pos.y < next_pos.y)
				offset.y = min(mv_spd, (float)(next_pos.y - pos.y));
			//Moving left
			if (pos.x > next_pos.x)
				offset.x = -min(mv_spd, (float)(pos.x - next_pos.x));
			//Moving right
			else if (pos.x < next_pos.x)
				offset.x = min(mv_spd, (float)(next_pos.x - pos.x));

			//Normalize diagonal movement
			if (offset.x != 0 and offset.y != 0)
				offset /= sqrt2;

			Entity::MoveBy(Round(offset));
		}
		else
			path.pop();
	}
	else
		moving = false;
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
		stats.str = new_score;
		stats.fort = (stats.str + stats.con) * .5f;
		switch (stats.size) {
		case Size::Tiny:
			stats.self_weight = 1 + (stats.str);
			stats.max_carry_weight = max(3.f, stats.str * 10);
			break;

		case Size::Small:
			stats.self_weight = 75 + (stats.str * 10);
			stats.max_carry_weight = max(12.5f, stats.str * 50);
			break;

		case Size::Med:
			stats.self_weight = 150 + (stats.str * 10);
			stats.max_carry_weight = max(15.f, stats.str * 50);
			break;

		case Size::Big:
			stats.self_weight = 190 + (stats.str * 10);
			stats.max_carry_weight = max(17.5f, stats.str * 50);
			break;

		case Size::Large:
			stats.self_weight = 400 + (stats.str * 25);
			stats.max_carry_weight = max(25.f, stats.str * 100);
			break;

		case Size::Huge:
			stats.self_weight = 1000 + (stats.str * 100);
			stats.max_carry_weight = max(50.f, stats.str * 150);
			break;

		case Size::Massive:
			stats.self_weight = 2500 + (stats.str * 250);
			stats.max_carry_weight = max(75.f, stats.str * 200);
			break;

		case Size::Colossal:
			stats.self_weight = 10000 + (stats.str * 1000);
			stats.max_carry_weight = max(100.f, stats.str * 250);
			break;
		}
		//Determine flying speed
		SetFlySpeed();
		break;

		//Derived stats: fort, max_hlth, th_per_hlth, tw_per_hlth, tn_per_health
		//Setting Health is a special case, and is handled in SetMaxHealth()
	case Ab_Score::CON:
		stats.con = new_score;
		stats.fort = (stats.str + stats.con) * .5f;
		break;

		//Derived stats: Ref, w_spd, dodge
	case Ab_Score::AGI:
		stats.agi = new_score;
		stats.ref = (stats.agi + stats.dex) * .5f;
		stats.w_spd = stats.base_spd + (.5f * stats.agi);
		stats.dodge = max(0.f, stats.dex + stats.agi + dodge_penalty);
		break;

		//Derived stats: Ref, less_action_time, dodge
	case Ab_Score::DEX:
		stats.dex = new_score;
		stats.ref = (stats.agi + stats.dex) * .5f;
		stats.dodge = max(0.f, stats.dex + stats.agi + dodge_penalty);
		break;

		//No derived stats
	case Ab_Score::INT:
		stats.intl = new_score;
		break;

		//Derived stats: Will
	case Ab_Score::WIS:
		stats.wis = new_score;
		stats.will = (stats.wis + stats.cha) * .5f;
		break;

		//Derived stats: Will
	case Ab_Score::CHA:
		stats.cha = new_score;
		stats.will = (stats.wis + stats.cha) * .5f;
		break;
	}
}

float Creature::GetAbilityScore(Ab_Score a_s) {
	switch (a_s) {
	case Ab_Score::STR:
		return stats.str;
		break;

	case Ab_Score::CON:
		return stats.con;
		break;

	case Ab_Score::AGI:
		return stats.agi;
		break;

	case Ab_Score::DEX:
		return stats.dex;
		break;

	case Ab_Score::INT:
		return stats.intl;
		break;

	case Ab_Score::WIS:
		return stats.wis;
		break;

	case Ab_Score::CHA:
		return stats.cha;
		break;
	}

	cout << "You done goofed" << endl;
	return 0.0f;
}

void Creature::SetMaxHealth() {
	float old_max = stats.max_hlth;
	//For sentients, max health depends on class and level
	if (stats.genus == Genus::Sentient) {
		switch (stats.clss) {
		case Class::None:
			stats.max_hlth = 5;
			break;

		case Class::Arcanist:
			stats.max_hlth = (stats.level * 6) + (stats.con * stats.level);
			break;

		case Class::Rogue:
			stats.max_hlth = (stats.level * 8) + (stats.con * stats.level);
			break;

		case Class::Warrior:
			stats.max_hlth = (stats.level * 10) + (stats.con * stats.level);
			break;
		}
	}

	//For non-Sentients, max health partially depends on size but is mostly arbitrary
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