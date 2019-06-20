#include "tileedit.hpp"
#include <iostream>

TileEdit::~TileEdit() {
}

TileEdit::TileEdit(Engine & engine)
    : menu(engine), cursor(engine), brush(engine.tilem)
{
    using namespace input;
    engine.inputm.push_context(cursor_ctx);
    engine.inputm.push_context(brush_ctx);
    engine.inputm.push_context(menu_ctx);

    /////////////////////////////////////////////////////////////////////////////// MENU SETUP

    menu_ctx.bind(Event{sf::Event::MouseMoved}, [&](const Event & event){
        auto p = event.get_mousepos_pixel();
        menu.update_mousepos(p);
        return menu.contains(p);
    });

    Event clickevnt{sf::Event::MouseButtonPressed};
    clickevnt.set_button(sf::Mouse::Left);
    menu_ctx.bind(clickevnt, [&](const Event & event){
        auto p = event.get_mousepos_pixel();
        return menu.click(p);
    });

    menu.tile_selected.add_callback("select", [&](Tile::ID id){
        cursor.set_tile_type(id);
    });

    /////////////////////////////////////////////////////////////////////////////// CURSOR SETUP

    Event synctile{sf::Event::MouseMoved};
    cursor_ctx.bind(synctile, [&](const Event & event){
        cursor.set_coordinate(event.get_mousepos_logic());
        return engine.inputm.is_button_pressed(sf::Mouse::Middle) == false;
    });

    /////////////////////////////////////////////////////////////////////////////// BRUSH SETUP

    Event edit_tile{sf::Event::MouseButtonPressed};
    edit_tile.set_button(sf::Mouse::Left);
    brush_ctx.bind(edit_tile, [&](const Event & event){
        brush.add_tile(cursor.get_id(), cursor.get_coordinate());
        return true;
    });

    edit_tile.set_button(sf::Mouse::Right);
    brush_ctx.bind(edit_tile, [&](const Event & event){
        brush.remove_tile(event.get_mousepos_logic().to_grid());
        return true;
    });

    Event debug{sf::Event::KeyPressed};
    debug.set_key(sf::Keyboard::Z);
    brush_ctx.bind(debug, [&](const Event & event){
        std::cout << event.get_mousepos_logic().to_grid().info() << std::endl;
        return false;
    });

    Event paint{sf::Event::MouseMoved};
    brush_ctx.bind(paint, [&](const Event & event){
        if (engine.inputm.is_button_pressed(sf::Mouse::Left)) {
            brush.add_tile(cursor.get_id(), cursor.get_coordinate());
        }
        else if (engine.inputm.is_button_pressed(sf::Mouse::Right)) {
            brush.remove_tile(cursor.get_coordinate().to_grid());
        }
        return false;
    });
}
