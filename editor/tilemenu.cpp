#include "tilemenu.hpp"
#include <iostream>

void TileMenu::cleanup() {
    UI::Container::cleanup();
    for (auto p : element_ptrs) {
        delete p;
    }
    element_ptrs.clear();
}

TileMenu::~TileMenu() {
    cleanup();
}

TileMenu::TileMenu(Engine & engine) : EditorMenu(engine)
{
    int w = 128, h = config::winh, columns = 2;
    sf::Vector2f origin{0, 0};

    background = engine.spritef.create(engine.uirender, "tilemenu-bg");
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

        Tile tile = engine.tilef.create(engine.uirender, tile_id);
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
                                                         "tilemenu-selected");
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
