#include "PartyMember.h"
#include "../../Core/Input.h" //Window

PartyMember::PartyMember(Game* g, Scene* s, const Sprite::Info& s_i, const Stats& init_stats,
	const string por_name, const bool init_biped, const bool init_winged)
	: Creature(g, s, s_i, init_stats, por_name, init_biped, init_winged) {

	//The health bar
	hlth_bar.w = round(portrait.GetSprSize().x * .98f);
	hlth_bar.h = round(portrait.GetSprSize().y * .14f);

	//The health text
	Text::Info h_t_info = {};
	h_t_info.str = to_string(stats.hlth) + "/" + to_string(stats.max_hlth);
	h_t_info.max_width = hlth_bar.w;
	hlth_txt.Init(&game->default_fonts[36], h_t_info);
}

void PartyMember::GetInput() {
	//Become selected if our bounding box or portrait is clicked on
	//Bbox uses world mouse coordinates, portrait uses screen mouse coordinates
	if ((Collision::Point(Input::MousePos(), bbox) or Collision::Point(Input::MousePos(), por_bbox)) and Input::BtnPressed(LMB)) {
		//We are not using the area selection function
		scene->selecting = false;

		//If we aren't holding shift, deselect all party members before selecting this one
		if (!(Input::KeyDown(LSHIFT) or Input::KeyDown(RSHIFT))) {
			vector<shared_ptr<PartyMember>> p_ms = scene->GetPartyMems();
			for (auto& p_m : p_ms) p_m->selected = false;
		}
		selected = !(Input::KeyDown(LCTRL) or Input::KeyDown(RCTRL));
	}
}

void PartyMember::Update() {
	Creature::Update();

	//Update portrait and related details position
	//Position is just the bottom left for Adventure 1
	portrait.MoveTo(Round(portrait.GetSprSize().x * .25f + game->camera.viewport.x, game->resolution.y - portrait.GetSprSize().y * 1.25f + game->camera.viewport.y));
	por_bbox.x = portrait.GetPos().x - round(portrait.GetSprSize().x * .025f);
	por_bbox.y = portrait.GetPos().y - round(portrait.GetSprSize().y * .025f);
	nameplate.MoveTo({ portrait.GetPos() });
	//Update hlth_bar size (TO-DO)
	hlth_bar.x = portrait.GetPos().x + round(portrait.GetSprSize().x * .01f);
	hlth_bar.y = portrait.GetPos().y + round(portrait.GetSprSize().y * .85f);
	hlth_txt.info.str = to_string((int)stats.hlth) + "/" + to_string((int)stats.max_hlth);
	hlth_txt.MoveTo({ hlth_bar.x, hlth_bar.y - 6 });
}

void PartyMember::Draw() {
	Creature::Draw();
	if (scene->label == Scenes::AREA) {
		//Party members draw their portraits and health bars at all times
		//Draw the highlight if selected first, then the portrait
		if (selected)
			game->renderer.DrawRect(por_bbox, Color(1));
		game->renderer.DrawSprite(portrait);
		game->renderer.DrawTxt(nameplate);

		//Draw the missing health bar first, then the remaining health bar over it
		game->renderer.DrawRect(Rect{ {hlth_bar.x, hlth_bar.y}, {hlth_bar.w, hlth_bar.h} }, Color(1,0,0));
		game->renderer.DrawRect(hlth_bar, Color(0,1,0));

		//Draw our remaining health numerically last
		game->renderer.DrawTxt(hlth_txt);

	}
}

