#include "Confirm.h"

Confirm::Confirm(const Engine e, Menu& m, const AnimInfo& a_i, const Animation::Transform& t, const UI::Style& style,
    const string conf_str, const uint init_ui_layer, const int init_dfc) :
    UI(e, m, a_i, t, style, init_ui_layer, init_dfc) {

    uint scale = engine.game.GetResScale();
    Text::Init(label, engine.game.default_font, style.font_size, sf::Vector2i(pos.x, pos.y - scale * 40), conf_str, {.5f, .5f}, 200*scale);

    auto btn = make_shared<Button>(
        e, menu,
        AnimInfo{ "UI/Button", 93, 26 },
        Animation::Transform{ sf::Vector2i(pos.x, pos.y - scale * 10), {.5f, .5f}, {scale, scale} },
        UI::Style{ UIElems::YES, scale * 16 },
        ui_layer, dfc-1);
    menu.AddUIElem(btn); //Adds to scene as well
    btn = make_shared<Button>(
        e, menu,
        AnimInfo{ "UI/Button", 93, 26 },
        Animation::Transform{ sf::Vector2i(pos.x, pos.y + scale * 30), {.5f, .5f}, {scale, scale} },
        UI::Style{ UIElems::NO, scale * 16 },
        ui_layer, dfc - 1);
    menu.AddUIElem(btn); //Adds to scene as well
}

void Confirm::Draw() {
    Entity::Draw();
    engine.window.draw(label);
}
