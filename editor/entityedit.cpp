#include "entityedit.hpp"

EntityEdit::~EntityEdit() {
}

EntityEdit::EntityEdit(Engine & engine) : menu(engine), cursor(engine), brush(engine.entitym)
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

    menu.entity_selected.add_callback("select", [&](const std::string & name){
        cursor.set_entity_type(name);
    });

    /////////////////////////////////////////////////////////////////////////////// CURSOR SETUP

    Event syncentity{sf::Event::MouseMoved};
    cursor_ctx.bind(syncentity, [&](const Event & event){
        auto pos = event.get_mousepos_logic();
        cursor.update_mousepos(pos);
        return engine.inputm.is_button_pressed(sf::Mouse::Middle) == false;
    });

    /////////////////////////////////////////////////////////////////////////////// BRUSH SETUP

    Event edit_entity{sf::Event::MouseButtonPressed};
    edit_entity.set_button(sf::Mouse::Left);
    brush_ctx.bind(edit_entity, [&](const Event & event){
        brush.add_entity(cursor.get_type(), cursor.get_coordinate());
        return true;
    });
    edit_entity.set_button(sf::Mouse::Right);
    brush_ctx.bind(edit_entity, [&](const Event & event){
        //brush.remove_entity(cursor.get_coordinate());
        return true;
    });
}

