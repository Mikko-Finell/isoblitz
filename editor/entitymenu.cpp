#include "entitymenu.hpp"
#include <iostream>

void EntityMenu::cleanup() {
    UI::Container::cleanup();
    for (auto p : element_ptrs) {
        delete p;
    }
    element_ptrs.clear();
}

EntityMenu::~EntityMenu() {
    cleanup();
}

EntityMenu::EntityMenu(Engine & engine) : EditorMenu(engine) 
{
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
    auto entity_names = engine.entityf.get_all();
    for (auto & name : entity_names) {
        auto button = new EntityMenuItem;
        add_element(button);

        Entity entity = engine.entityf.create(engine.uirender, name);
        button->sprite.idle = entity.animation.sprite;

        button->id(1);
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

        button->clicked.add_callback("btnclick", [name,this](auto &&){
            entity_selected(name);
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
