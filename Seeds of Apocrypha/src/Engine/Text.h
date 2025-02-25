#pragma once
#include <iostream>
#include "Utils/Macros.h"

namespace Text {
    void Init(sf::Text& text, sf::Font& font, const uint char_size = 36, const sf::Vector2f pos = {0, 0}, const std::string str = "DEFAULT", const sf::Vector2f ori = {.5f, .5f});
    void SetStr(sf::Text& text, std::string str, const float max_width = 1920.f);
    void SetCharSize(sf::Text& text, uint char_size);
    void SetOrigin(sf::Text& text, sf::Vector2f ori = { .5f, .5f });
    sf::Vector2f GetOrigin(sf::Text& text);
}