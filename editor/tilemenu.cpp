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

        Tile tile = engine.tilef.create_unmanaged(engine.uirender, tile_id);
        button->sprite.idle = tile.sprite;

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

        button->clicked.add_callback("btnclick", [this](Tile::ID id){
            tile_selected(id);
        });

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
        delete p;
    }
}

void TileMenu::update_mousepos(const Position & pos) {
    for (auto p : element_ptrs) {
        p->update_mousepos(pos);
    }
}
