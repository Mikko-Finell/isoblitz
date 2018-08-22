#include "entitymenu.hpp"
#include <iostream>

EntityMenuItem::EntityMenuItem(const Entity & entity) {
    this->entity = entity;
    //sprite.tile.set_data(tile.get_spritedata());
}

void EntityMenuItem::init(SpriteFactory & sf, RenderSystem & rs) {
    sprite.selected = sf.get("editor-ui", "tilemenu-selected");
    sprite.hovering = sf.get("editor-ui", "tilemenu-hovering");

    sprite.hovering.set_layer(UI_LAYER + 1);
    sprite.selected.set_layer(UI_LAYER + 2);
    entity.animation.sprite.set_layer(UI_LAYER + 3);

    //sprite.tile.init(rs);
    sprite.hovering.init(rs);
    sprite.selected.init(rs);

    sprite.hovering.hide();
    sprite.selected.hide();
}

void EntityMenuItem::set_screencoords(const sf::IntRect & rect) {
    entity.animation.sprite.set_screencoords(rect);
    sprite.hovering.set_screencoords(rect);
    sprite.selected.set_screencoords(rect);
    this->rect = rect;
}

void EntityMenuItem::update_mousepos(const Position & p) {
    if (rect.contains(p)) {
        sprite.hovering.show();
    }
    else {
        sprite.hovering.hide();
    }
}

bool EntityMenuItem::try_click(const Position & p) {
    sprite.selected.hide();
    if (rect.contains(p)) {
        sprite.selected.show();
        clicked(entity.get_type());
        return true;
    }
    return false;
}

// EntityMenu /////////////////////////////////////////////////////////////////////

EntityMenu::EntityMenu(SpriteFactory & sf, RenderSystem & rs, 
        EntityFactory & ef, int w, int h, int c)
    : width(w), height(h), columns(c)
{
    background = sf.get("editor-ui", "tilemenu-bg");
    background.set_position(origin.x, origin.y);
    background.set_size(w, h);
    background.set_layer(UI_LAYER);
    background.init(rs);

    const int button_size = w / c;
    int x = origin.x;
    int y = origin.y;
    int col = 0, row = 0;
    auto es = ef.get_all();
    for (auto & entity : es) {
        buttons.emplace_back(entity);
        auto & button = buttons.back();
        button.clicked.add_callback([&](type_id_t id){ entity_selected(id); });
        button.init(sf, rs);
        button.set_screencoords(sf::IntRect{x, y, button_size, button_size});

        ++col;
        if (col == columns) {
            col = 0;
            ++row;
        }

        x = origin.x + col * button_size;
        y = origin.y + row * button_size;
    }
}

void EntityMenu::update_mousepos(const Position & p) {
    for (auto & button : buttons) {
        button.update_mousepos(p);
    }
}

bool EntityMenu::try_click(const Position & p) {
    bool b = false;
    for (auto & button : buttons) {
        b = button.try_click(p) || b;
    }
    return b;
}