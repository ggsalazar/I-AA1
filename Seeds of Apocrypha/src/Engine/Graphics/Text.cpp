#include <sstream>
#include "Text.h"

namespace Engine {

void Text::SetStr(std::string str) {
    /*
    //Get the text origin relative to the old string so we can set it relative to the new string
    Vector2f t_origin = GetOrigin();

    //If the string is too long, insert some newline characters
    istringstream parser(str);
    ostringstream buffer;
    string word = "", line_buffer = "";
    while (parser >> word) {
        word_text.setString(word);

        if (!line_buffer.empty()) {
            line_text.setString(line_buffer);
            line_buffer += " ";
        }

        if (line_text.getLocalBounds().size.x + word_text.getLocalBounds().size.x > info.max_width) {
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

    info.str = buffer.str();

    SetOrigin(t_origin);
    */
    info.str = str;
}

void Text::SetCharSize(uint char_size) {
    /*
    //Get the text origin relative to the old string so we can set it relative to the new string
    Vector2f t_origin = GetOrigin();

    text.setCharacterSize(char_size);

    SetOrigin(t_origin);
    */
}

void Text::SetOrigin(Vector2f ori) {
    /*
    //Text object origin is based on CURRENT string of the text, ergo must be updated constantly
    if (ori.x < 0.f or 1.f < ori.x) ori.x = 0.f;
    if (ori.y < 0.f or 1.f < ori.y) ori.y = 0.f;


    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({ bounds.position.x + bounds.size.x * ori.x, bounds.position.y + bounds.size.y * ori.y });
    */
}

}