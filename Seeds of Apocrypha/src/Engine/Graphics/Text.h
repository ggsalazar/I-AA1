#pragma once
#include <iostream>
#include <string>
#include "../Core/Geometry.h" //Includes Vector2
#include "Font.h"

namespace Engine {
class Text {
    friend class Font;
public:
    struct Info {
        Font* font; //Raw ptr is fine since Text doesn't own the Font object
        std::string str = "";
        Vector2u pos;
        uint size = 36;
        uint max_width = 1920;
        Color color{ 1 };
        Vector2f origin{ .5f };
        float rot;
    };
    Circle pos_debug;

    Text(const Info& i) :
        info(i) {
        SetCharSize(info.size);
        SetStr(info.str);
        pos_debug.pos = info.pos;
        pos_debug.r = 2;
    }
    virtual ~Text() {}

    inline void SetFont(Font* font) { info.font = font; }
    inline Font* GetFont() const { return info.font; }

    void SetStr(std::string str);
    inline std::string GetStr() const { return info.str; }

    inline void SetPos(Vector2u pos) { info.pos = pos; }
    inline Vector2u GetPos() const { return info.pos; }

    void SetCharSize(uint char_size);
    inline uint GetCharSize() const { return info.size; }

    inline void SetMWidth(uint m_width) { info.max_width = m_width; SetStr(info.str); }
    inline uint GetMWidth() const { return info.max_width; }

    inline void SetColor(Color color = { 1 }) { info.color = color; }
    inline Color GetColor() const { return info.color; }

    void SetOrigin(Vector2f ori = { .5f, .5f });
    inline Vector2f GetOrigin() const { return info.origin; }

    inline void SetRot(float rot = 0.f) { info.rot = rot; }
    inline float GetRot() const { return info.rot; }

protected:
    //Protected because for most fields, when they are set, there are ancillary functions that must also be performed
    Info info;
};
}