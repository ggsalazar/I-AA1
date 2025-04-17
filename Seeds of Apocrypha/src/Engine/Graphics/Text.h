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
        string str = ""; //Use SetStr()
        Vector2u pos;
        uint char_size = 36; //Use SetCharSize()
        Vector2u str_size; //Set in SetStr()
        uint max_width = 1920;
        Color color{ 1 };
        Vector2f origin{ .5f }; //Use SetOrigin()
        float rot;
    };
    Info info;
    Circle pos_debug;

    Text(const Info& i) :
        info(i) {
        pos_debug.pos = info.pos;
        pos_debug.r = 2;
    }
    virtual ~Text() {}

    virtual void SetCharSize(uint new_char_size) = 0;

    inline void SetOrigin(Vector2f ori = { .5f, .5f }) {
        if (ori.x < 0.f or 1.f < ori.x) ori.x = 0.f;
        if (ori.y < 0.f or 1.f < ori.y) ori.y = 0.f;
    }

};
}