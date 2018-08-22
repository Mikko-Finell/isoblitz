#include "selection.hpp"
#include "common/engine.hpp"
#include "CASE/timer.hpp"
#include <iostream>

int main() {
    auto timer = new CASE::ScopeTimer{"Create systems"};

    Engine engine;
    SelectionManager selectm{engine.wrender, engine.spritef};

    input::Context selectionctx;
    input::Context cameractx;

    delete timer;
    // System initiation ////////////////////////////////////////////////////////
    timer = new CASE::ScopeTimer{"Init systems"};

    engine.inputm.push_context(cameractx);
    engine.inputm.push_context(selectionctx);

    engine.load("../maps/tmp.bulletmap");

    delete timer;
    // Input mapping ////////////////////////////////////////////////////////////
    timer = new CASE::ScopeTimer{"Input mapping"};

    using namespace input;

    Event sel_event{sf::Event::MouseButtonPressed};
    sel_event.set_button(sf::Mouse::Left);
    selectionctx.bind(sel_event, [&](const Event & event){
        selectm.start(event.get_mousepos());
        return true;
    });

    sel_event = input::Event{sf::Event::MouseMoved};
    selectionctx.bind(sel_event, [&](const Event & event){
        if (engine.inputm.is_button_pressed(sf::Mouse::Left)) {
            selectm.update(event.get_mousepos());
            return true;
        }
        return false;
    });

    sel_event = input::Event{sf::Event::MouseButtonReleased};
    sel_event.set_button(sf::Mouse::Left);
    selectionctx.bind(sel_event, [&](const Event & event){
        selectm.select_current_rect();
        return true;
    });

    delete timer;
    // Testing //////////////////////////////////////////////////////////////////
    timer = new CASE::ScopeTimer{"Testing setup"};


    delete timer;
    // Main loop ////////////////////////////////////////////////////////////////

    engine.run();
}
