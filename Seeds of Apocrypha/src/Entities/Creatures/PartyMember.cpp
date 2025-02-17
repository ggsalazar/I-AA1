#include "PartyMember.h"

PartyMember::PartyMember(const Structure& s, const AnimInfo& a_i, const Animation::Transform& t,
	const Stats& init_stats, const string por_name, const bool init_biped, const bool init_winged, const int init_dfc) :
	Creature(s, a_i, t, init_stats, por_name, init_biped, init_winged, init_dfc) {

	//Set up the portrait highlighter
	por_highlight.setSize({ 34 * portrait.getScale().x, 34 * portrait.getScale().y });
	por_highlight.setFillColor(sf::Color::Green);
	por_highlight.setOrigin(sf::Vector2f(por_highlight.getSize() * .5f));
}

void PartyMember::Draw(const bool debug) {
	Creature::Draw(debug);
	//Set the camera to the hud
	structure.window.setView(structure.game.hud);
	
	//Party members draw their portraits at all times
	//Position is temporarily bottom left
	portrait.setPosition({ structure.game.camera.getViewport().position.x + por_highlight.getSize().x*.6f, structure.game.camera.getViewport().position.y + structure.game.camera.getSize().y - por_highlight.getSize().y*.6f});
	

	//If they're selected, they have a green border
	if (selected) {
		por_highlight.setPosition(portrait.getPosition());
		structure.window.draw(por_highlight);
	}


	structure.window.draw(portrait);
	
	//Set the camera back to the world
	structure.window.setView(structure.game.camera);
}
