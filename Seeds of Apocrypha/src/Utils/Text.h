#pragma once
#include <iostream>
#include "Macros.h"

namespace Text {
    void Init(sf::Text& text, sf::Font& font, const uint char_size, const sf::Vector2f pos, const std::string str, const sf::Vector2f ori = {.5f, .5f});
    void SetStr(sf::Text& text, std::string str, const float max_width = 1920.f);
    void SetCharSize(sf::Text& text, uint char_size);
    void SetOrigin(sf::Text& text, sf::Vector2f ori = { .5f, .5f });
    sf::Vector2f GetOrigin(sf::Text& text);
}