#include "tilemenu.hpp"
#include <iostream>
/*

TileMenuItem::TileMenuItem(const Tile & tile) {
    this->tile = tile;
    sprite.tile = tile.sprite;
}

void TileMenuItem::init(SpriteFactory & sf, RenderSystem & rs) {
    sprite.selected = sf.create(rs, "editor-ui", "tilemenu-selected");
    sprite.hovering = sf.create(rs, "editor-ui", "tilemenu-hovering");

    sprite.tile->set_layer(config::ui_layer + 3);
    sprite.selected->set_layer(config::ui_layer + 2);
    sprite.hovering->set_layer(config::ui_layer + 1);
}

void TileMenuItem::set_screencoords(const sf::FloatRect & rect) {
    sprite.tile->set_screencoords(rect);
    sprite.hovering->set_screencoords(rect);
    sprite.selected->set_screencoords(rect);
    this->rect = rect;
}

void TileMenuItem::update_mousepos(const Position & p) {
    if (rect.contains(p)) {
        sprite.hovering.show();
    }
    else {
        sprite.hovering.hide();
    }
}

bool TileMenuItem::try_click(const Position & p) {
    sprite.selected.hide();
    if (rect.contains(p)) {
        sprite.selected.show();
        clicked(tile.get_id());
        return true;
    return false;
}

// TileMenu /////////////////////////////////////////////////////////////////////

TileMenu::TileMenu(SpriteFactory & sf, RenderSystem & rs, TileFactory & tilef,
        int w, int h, int c)
    : width(w), height(h), columns(c)
{
    background = sf.create(rs, "editor-ui", "tilemenu-bg");
    background->set_position(origin.x, origin.y);
    background->set_size(w, h);
    background->set_layer(config::ui_layer);

    const int button_size = w / c;
    int x = origin.x;
    int y = origin.y;
    int col = 0, row = 0;
    auto tiles = tilef.get_all();
    for (auto & tile : tiles) {
        buttons.emplace_back(tile);
        auto & button = buttons.back();
        button.clicked.add_callback("button_clicked", [&](tile_id_t id){
            tile_selected(id);
        });

        button.init(sf, rs);
        button.set_screencoords(sf::FloatRect(x, y, button_size, button_size));
        ++col;
        if (col == columns) {
            col = 0;
            ++row;
        }

        x = origin.x + col * button_size;
        y = origin.y + row * button_size;
    }
}

void TileMenu::update_mousepos(const Position & p) {
    for (auto & button : buttons) {
        button.update_mousepos(p);
    }
}

bool TileMenu::try_click(const Position & p) {
    bool b = false;
    for (auto & button : buttons) {
        b = button.try_click(p) || b;
    }
    return b;
}
*/
