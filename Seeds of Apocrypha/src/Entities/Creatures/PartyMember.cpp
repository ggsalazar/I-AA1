#include "PartyMember.h"

PartyMember::PartyMember(Game& g, Scene* s, const Sprite::Info& s_i, const Stats& init_stats,
	const string por_name, const bool init_biped, const bool init_winged)
	: Creature(g, s, s_i, init_stats, por_name, init_biped, init_winged) {
	
	//The health bar
	hlth_bar.w = 34 * portrait->GetSprSize().x;
	hlth_bar.h = 6 * portrait->GetSprSize().y;

	//The health text
	Text::Info h_t_info = {};
	h_t_info.str = to_string(stats.hlth) + "/" + to_string(stats.max_hlth);
	h_t_info.max_width = hlth_bar.w;
	hlth_txt = make_u<Text>(game.default_font36.get(), h_t_info);
}

void PartyMember::GetInput() {
	//Become selected if our bounding box or portrait is clicked on
	//Bbox uses world mouse coordinates, portrait uses screen mouse coordinates
	if ((bbox.contains(engine.window.mapPixelToCoords(MOUSEPOS_E)) or por_bbox.contains(Vector2f(MOUSEPOS_E))) and Input::KeyPressed(LMB)) {
		//We are not using the area selection function
		engine.scene->selecting = false;

		//If we aren't holding shift, deselect all party members before selecting this one
		if (!(BUTTONDOWN(LSHIFT) or BUTTONDOWN(RSHIFT))) {
			vector<shared_ptr<PartyMember>> p_ms = engine.scene->GetPartyMems();
			for (auto& p_m : p_ms) p_m->selected = false;
		}
		selected = !(BUTTONDOWN(LCTRL) or BUTTONDOWN(RCTRL));
	}
}

void PartyMember::Update() {
	Creature::Update();

	//Update portrait and related details position
	//Position is just the bottom left for Seeds of Apocrypha
	Vector2i cam_bot_left = { round(engine.game.camera.getCenter().x - engine.game.camera.getSize().x * .5f), round(engine.game.camera.getCenter().y + engine.game.camera.getSize().y * .5f) };
	portrait.setPosition({ round(cam_bot_left.x + portrait.getGlobalBounds().size.x*1.3f), round(cam_bot_left.y-portrait.getGlobalBounds().size.y*1.3f)});
	por_bbox.position.x = portrait.getPosition().x - round(por_bbox.size.x * .5f);
	por_bbox.position.y = portrait.getPosition().y - round(por_bbox.size.y * .5f);
	por_highlight.setPosition(portrait.getPosition());
	nameplate.setPosition({ round(portrait.getPosition().x - portrait.getGlobalBounds().size.x * .5f), round(portrait.getPosition().y - portrait.getGlobalBounds().size.y * .5f) });
	mssng_hlth_bar.setPosition({ round(portrait.getPosition().x - por_highlight.getSize().x * .5f), round(portrait.getPosition().y + portrait.getGlobalBounds().size.y * .5f)});
	//Update hlth_bar size (to-do)
	hlth_bar.setPosition(mssng_hlth_bar.getPosition());
	Text::SetStr(hlth_txt, to_string((int)stats.hlth) + "/" + to_string((int)stats.max_hlth));
	hlth_txt.setPosition({round(portrait.getPosition().x), round(portrait.getPosition().y + portrait.getGlobalBounds().size.y * .6f) });
	std::cout << "Health text position: " << hlth_txt.getPosition() << endl;
}

void PartyMember::Draw() {
	Creature::Draw();
	if (engine.scene->label == Scenes::AREA) {
		//Party members draw their portraits and health bars at all times
		//Draw the highlight if selected first, then the portrait
		if (selected)
			engine.window.draw(por_highlight);
		engine.window.draw(portrait);
		engine.window.draw(nameplate);

		//Draw the missing health bar first, then the remaining health bar over it
		engine.window.draw(mssng_hlth_bar);
		engine.window.draw(hlth_bar);

		//Draw our remaining health numerically last
		engine.window.draw(hlth_txt);
	}
}

