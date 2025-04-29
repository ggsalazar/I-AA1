#include "Confirm.h"

Confirm::Confirm(Game& g, Scene* s, Menu& m, const Sprite::Info& s_i, const UI::Style& style,
    const string conf_str, const uint init_ui_layer) :
    UI(e, m, a_i, t, style, init_ui_layer, init_dfc) {

    uint scale = game.GetResScale();
    Text::Init(label, game.default_font, style.font_size, Vector2u(pos.x, pos.y - scale * 40), conf_str, {.5f, .5f}, 200*scale);

    auto btn = make_shared<Button>(
        e, menu,
        AnimInfo{ "UI/Button", 93, 26 },
        Animation::Transform{ Vector2u(pos.x, pos.y - scale * 10), {.5f, .5f}, {scale, scale} },
        UI::Style{ UIElems::YES, scale * 16 },
        ui_layer, dfc-1);
    menu.AddUIElem(btn); //Adds to scene as well
    btn = make_shared<Button>(
        e, menu,
        AnimInfo{ "UI/Button", 93, 26 },
        Animation::Transform{ Vector2u(pos.x, pos.y + scale * 30), {.5f, .5f}, {scale, scale} },
        UI::Style{ UIElems::NO, scale * 16 },
        ui_layer, dfc - 1);
    menu.AddUIElem(btn); //Adds to scene as well
}

void Confirm::Draw() {
    Entity::Draw();
    game.renderer->DrawTxt(*label);
}
