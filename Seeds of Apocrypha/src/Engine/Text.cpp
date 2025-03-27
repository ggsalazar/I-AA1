#include <sstream>
#include "Text.h"

void Text::Init(sf::Text& text, sf::Font& font, const uint char_size, const sf::Vector2f pos, std::string str, const sf::Vector2f ori, const float max_width) {
    text.setFont(font);
    text.setCharacterSize(char_size);
    SetStr(text, str, max_width);
    SetOrigin(text, ori);
    text.setPosition(pos);
}

void Text::SetStr(sf::Text& text, std::string str, const float max_width) {
    //Get the text origin relative to the old string so we can set it relative to the new string
    sf::Vector2f t_origin = GetOrigin(text);

    //If the string is too long, insert some newline characters
    std::istringstream parser(str);
    std::ostringstream buffer;
    std::string word, line_buffer;
    sf::Text word_text(text), line_text(text);
    word_text.setString(""); line_text.setString("");
    while (parser >> word) {
        word_text.setString(word);

        if (!line_buffer.empty()) {
            line_text.setString(line_buffer);
            line_buffer += " ";
        }

        if (line_text.getLocalBounds().size.x + word_text.getLocalBounds().size.x > max_width) {
            //Remove the trailing space
            line_buffer.pop_back();
            buffer << line_buffer << "\n";
            line_buffer = word;
        }
        else
            line_buffer += word;

    }
    //Append the last line
    buffer << line_buffer;

    text.setString(buffer.str());

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