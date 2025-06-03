#include "PartyMember.h"

PartyMember::PartyMember(const Sprite::Info& s_i, const Stats& init_stats,
	const string por_name, const bool init_biped, const bool init_winged)
	: Creature(s_i, init_stats, por_name, 100, init_biped, init_winged) {
	
	//The health bar
	hlth_bar.w = round(portrait.GetSprSize().x * .98f);
	hlth_bar.h = round(portrait.GetSprSize().y * .14f);

	//The health text
	Text::Info h_t_info = {};
	h_t_info.str = to_string(stats.hlth) + "/" + to_string(stats.max_hlth);
	h_t_info.max_width = hlth_bar.w;
	hlth_txt.Init(&game->default_fonts[36], h_t_info);

	//Camera sprite
	Sprite::Info c_s_info = {};
	c_s_info.sheet = "UI/Cam"; c_s_info.frame_size = { 12, 12 }; c_s_info.scale = game->GetResScale();
	cam_sprite.Init(c_s_info);
	cam_sprite_bbox = { {0}, cam_sprite.GetSprSize() };
}

void PartyMember::GetInput() {
	Creature::GetInput();
	//Become selected if our bounding box or portrait is clicked on
	//Bbox uses world mouse coordinates, portrait uses screen mouse coordinates
	if ((Collision::Point(Input::MousePos(), bbox) or Collision::Point(Input::MousePos(), por_bbox)) and Input::BtnPressed(LMB)) {
		//We are not using the area selection function or the default lmb action
		scene->selecting = false;
		scene->lmb_action = false;

		//If we aren't holding shift, deselect all party members before selecting this one
		if (!(Input::KeyDown(LSHIFT) or Input::KeyDown(RSHIFT))) {
			for (auto& p_m : scene->GetPartyMems()) p_m->selected = false;
		}
		selected = !(Input::KeyDown(LCTRL) or Input::KeyDown(RCTRL));
	}

	//Become part of the cam_locked gang
	if (!scene->selecting and Collision::Point(Input::MousePos(), cam_sprite_bbox)) {
		scene->lmb_action = false;
		if (Input::BtnPressed(LMB)) {
			cam_locked = !cam_locked;

			if (cam_locked)
				cam_sprite.SetColor(Color(0, 1, 0));
			else cam_sprite.SetColor(Color(1));
		}
	}
}

void PartyMember::Update() {
	Creature::Update();

	//Update portrait and related details position
	//Position is just the bottom left
	//Portrait
	portrait.MoveTo(Round(portrait.GetSprSize().x * .25f + game->camera.viewport.x, game->resolution.y - portrait.GetSprSize().y * 1.25f + game->camera.viewport.y));
	por_bbox.x = portrait.GetPos().x - round(portrait.GetSprSize().x * .025f);
	por_bbox.y = portrait.GetPos().y - round(portrait.GetSprSize().y * .025f);
	//Cam locking sprite
	//This assumes the origin for both sprites is top-left; should rewrite to account for all possible origins eventually
	cam_sprite.MoveTo({ portrait.GetPos().x, portrait.GetPos().y - cam_sprite.GetSprSize().y - game->GetResScale()*2});
	cam_sprite_bbox.x = cam_sprite.GetPos().x;
	cam_sprite_bbox.y = cam_sprite.GetPos().y;
	//Nameplate
	nameplate.MoveTo(portrait.GetPos());
	//Health Bar
	//Update hlth_bar size (TO-DO)
	hlth_bar.x = portrait.GetPos().x + round(portrait.GetSprSize().x * .01f);
	hlth_bar.y = portrait.GetPos().y + round(portrait.GetSprSize().y * .85f);
	hlth_txt.info.str = to_string((int)stats.hlth) + "/" + to_string((int)stats.max_hlth); //These are int temporarily
	hlth_txt.MoveTo({ hlth_bar.x, hlth_bar.y - 6 });
}

void PartyMember::Draw() {

	//Draw the last step in their path so the player always knows where they're headed
	if (!path.empty()) {
		Rect last_step = { path.back() - TS*.5, TS };
		game->renderer.DrawRect(last_step, Color(0,0), Color(1,0,0, .4f));
	}

	Creature::Draw();
}

void PartyMember::DrawPortrait() {
	//Camera locking sprite
	game->renderer.DrawSprite(cam_sprite);
	if (Collision::Point(Input::MousePos(), cam_sprite_bbox))
		game->renderer.DrawRect(cam_sprite_bbox, Color(0,0), Color(1));

	//Party members draw their portraits and health bars at all times
	//Draw the highlight if selected first, then the portrait
	if (selected)
		game->renderer.DrawRect(por_bbox, Color(1));
	game->renderer.DrawSprite(portrait);
	game->renderer.DrawTxt(nameplate);

	//Draw the missing health bar first, then the remaining health bar over it
	game->renderer.DrawRect(Rect{ {hlth_bar.x, hlth_bar.y}, {hlth_bar.w, hlth_bar.h} }, Color(1, 0, 0));
	game->renderer.DrawRect(hlth_bar, Color(0, 1, 0));

	//Draw our remaining health numerically last
	game->renderer.DrawTxt(hlth_txt);
}

