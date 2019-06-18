#include "tilemenu.hpp"
#include <iostream>

void TileMenuItem::activate() {
    clicked(_id);
}

void TileMenuItem::init() {
}

void TileMenuItem::update_mousepos(const Position & pos) {
    UI::Element::update_mousepos(pos);
    sprite.hovering.show(activated == true or hovering == true);
}

bool TileMenuItem::contains(const Position & pos) {
    return rect.contains(pos);
}

void TileMenuItem::click(const Position & pos) {
    UI::Element::click(pos);
    sprite.activated.show(activated == true);
    sprite.hovering.show(activated == true or hovering == true);
}

/////////////////////////////////////////////////////////////////////

TileMenu::TileMenu(Engine & engine) : UI::Container(engine) {

    int w = 128, h = config::winh, columns = 2;
    sf::Vector2f origin{0, 0};

    background = engine.spritef.create(engine.uirender, "editor-ui", "tilemenu-bg");
    background.set_size(w, h);
    background.set_position(origin.x, origin.y);
    background.set_layer(config::ui_layer);

    const int button_size = w / columns;
    int x = origin.x;
    int y = origin.y;
    int col = 0, row = 0;
    auto tiles = engine.tilef.get_all();
    for (auto & tile_id : tiles) {
        auto button = new TileMenuItem;
        add_element(button);

        Tile * tile = engine.tilef.create(engine.uirender, tile_id);
        button->sprite.idle = tile->sprite;
        engine.tilem.destroy(tile);
        button->id(tile_id);
        button->rect = sf::FloatRect(x, y, button_size, button_size);

        button->sprite.idle
            .set_offset(0, 0)
            .set_size(button_size, button_size)
            .set_position(x, y)
            .set_layer(config::ui_layer - 1);

        button->sprite.hovering = button->sprite.idle;
        button->sprite.hovering.set_layer(config::ui_layer + 2);
        button->sprite.hovering.hide();

        button->sprite.activated = engine.spritef.create(engine.uirender, 
                                                         "editor-ui", "tilemenu-selected");
        button->sprite.activated
            .set_offset(0, 0)
            .set_size(button_size, button_size)
            .set_position(x, y)
            .set_layer(config::ui_layer + 1);

        button->sprite.activated.hide();

        ++col;
        if (col == columns) {
            col = 0;
            ++row;
        }

        x = origin.x + col * button_size;
        y = origin.y + row * button_size;
    }
}

void TileMenu::cleanup() {
    UI::Container::cleanup();
    for (auto p : element_ptrs) {
        //delete p;
    }
}

void TileMenu::update_mousepos(const Position & pos) {
    for (auto p : element_ptrs) {
        p->update_mousepos(pos);
    }
}


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
