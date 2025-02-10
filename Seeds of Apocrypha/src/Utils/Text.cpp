#include "Text.h"

void Text::Init(sf::Text& text, sf::Font& font, const uint char_size, const sf::Vector2f pos, std::string str, const sf::Vector2f ori) {
    text.setFont(font);
    text.setCharacterSize(char_size);
    text.setString(str);
    SetOrigin(text, ori);
    text.setPosition(pos);
}

void Text::SetStr(sf::Text& text, std::string str, const float max_width) {
    //If the string is too long, insert some newline characters
    //float str_width =

    //Get the text origin relative to the old string so we can set it relative to the new string
    sf::Vector2f t_origin = GetOrigin(text);

    text.setString(str);

    SetOrigin(text, t_origin);
}

void Text::SetCharSize(sf::Text& text, uint char_size) {
    //Get the text origin relative to the old string so we can set it relative to the new string
    sf::Vector2f t_origin = GetOrigin(text);

    text.setCharacterSize(char_size);

    SetOrigin(text, t_origin);
}

void Text::SetOrigin(sf::Text& text, sf::Vector2f ori) {
    //Text object origin is based on CURRENT string of the text, ergo must be updated constantly
    ori.x = ori.x <= 1.f ? ori.x : 0.f;
    ori.y = ori.y <= 1.f ? ori.y : 0.f;


    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({ bounds.position.x + bounds.size.x * ori.x, bounds.position.y + bounds.size.y * ori.y });
}

sf::Vector2f Text::GetOrigin(sf::Text& text) {
    sf::FloatRect bounds = text.getLocalBounds();
    sf::Vector2f origin = text.getOrigin();

    //Normalize
    origin.x = origin.x / bounds.size.x;
    origin.y = (origin.y - bounds.position.y) / bounds.size.y;

    return origin;
}