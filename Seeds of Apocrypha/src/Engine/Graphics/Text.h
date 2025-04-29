#pragma once
#include <iostream>
#include <string>
#include "../Core/Geometry.h" //Includes Vector2
#include "Font.h"

using std::string;

namespace Engine {
class Text {
    friend class Font;
public:
    struct Info {
        string str = "DEFAULT";
        Vector2i pos; //Use MoveTo()/MoveBy()
        uint char_size = 36;
        Vector2u str_size;
        uint max_width = 1920;
        Color color{ 1 };
        Vector2f origin{}; //Use SetOrigin()
        float rot = 0.f;
    };
    Font* font; //Raw pointer is fine since Text doesn't own the font object
    Info info;
    Circle pos_debug;

    Text(Font* f = nullptr, const Info& i = {}) :
        font(f), info(i) {
        pos_debug.pos = info.pos;
        pos_debug.r = 2;
    }
    virtual ~Text() {}

    inline void MoveTo(Vector2i new_pos) { info.pos = new_pos; pos_debug.pos = info.pos; }
    inline void MoveBy(Vector2i offset) { info.pos += offset; pos_debug.pos = info.pos; }

    inline void SetOrigin(Vector2f ori = { .5f, .5f }) {
        if (ori.x < 0.f or 1.f < ori.x) ori.x = 0.f;
        if (ori.y < 0.f or 1.f < ori.y) ori.y = 0.f;

        info.origin = ori;
    }

};
}