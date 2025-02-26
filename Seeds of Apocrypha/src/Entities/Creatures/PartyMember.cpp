#include "PartyMember.h"

PartyMember::PartyMember(const Engine& s, const AnimInfo& a_i, const Animation::Transform& t,
	const Stats& init_stats, const string por_name, const bool init_biped, const bool init_winged, const int init_dfc) :
	Creature(s, a_i, t, init_stats, por_name, init_biped, init_winged, init_dfc), hlth_txt(engine.game.default_font) {

	//Set up the portrait highlighter
	por_highlight.setSize(sf::Vector2f(34.f * portrait.getScale()));
	por_highlight.setFillColor(sf::Color::Green);
	por_highlight.setOrigin(sf::Vector2f(por_highlight.getSize() * .5f));

	//The missing health bar, a static red bar drawn under the health bar
	mssng_hlth_bar.setSize({ 34 * portrait.getScale().x, 6 * portrait.getScale().y });
	mssng_hlth_bar.setFillColor(sf::Color::Red);
	//Origin is 0,0
	
	//The health bar
	hlth_bar.setSize(mssng_hlth_bar.getSize());
	hlth_bar.setFillColor(sf::Color::Green);
	//Origin is 0,0

	//The health text
	Text::Init(hlth_txt, engine.game.default_font, engine.game.GetResScale() * 10, portrait.getPosition(), to_string(stats.hlth) + "/" + to_string(stats.max_hlth));

}

void PartyMember::GetInput() {
	//Become selected if our bounding box or portrait is clicked on
	//Bbox uses world mouse coordinates, portrait uses screen mouse coordinates
	if ((bbox.contains(engine.window.mapPixelToCoords(MOUSEPOS_E)) or por_bbox.contains(sf::Vector2f(MOUSEPOS_E))) and Input::KeyPressed(LMB)) {
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
	sf::Vector2f cam_bot_left = { engine.game.camera.getCenter().x - engine.game.camera.getSize().x * .5f, engine.game.camera.getCenter().y + engine.game.camera.getSize().y * .5f };
	portrait.setPosition({ cam_bot_left.x + portrait.getGlobalBounds().size.x*1.3f, cam_bot_left.y-portrait.getGlobalBounds().size.y*1.3f});
	por_bbox.position.x = portrait.getPosition().x - por_bbox.size.x * .5f;
	por_bbox.position.y = portrait.getPosition().y - por_bbox.size.y * .5f;
	por_highlight.setPosition(portrait.getPosition());
	nameplate.setPosition({ portrait.getPosition().x - portrait.getGlobalBounds().size.x * .5f, portrait.getPosition().y - portrait.getGlobalBounds().size.y * .5f });
	mssng_hlth_bar.setPosition({ portrait.getPosition().x - por_highlight.getSize().x * .5f, portrait.getPosition().y + portrait.getGlobalBounds().size.y * .5f});
	//Update hlth_bar size (to-do)
	hlth_bar.setPosition(mssng_hlth_bar.getPosition());
	Text::SetStr(hlth_txt, to_string((int)stats.hlth) + "/" + to_string((int)stats.max_hlth));
	hlth_txt.setPosition({portrait.getPosition().x, portrait.getPosition().y + portrait.getGlobalBounds().size.y * .6f });
}

void PartyMember::Draw() {
	Creature::Draw();
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

