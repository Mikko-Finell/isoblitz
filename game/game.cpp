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

    auto scroll = [&](const input::Event & event){
        if (engine.inputm.is_button_pressed(sf::Mouse::Middle)) {
            // note: mousedt is the actual change, and so moving the camera by
            // that amount makes it look like the world is scrolling in the 
            // opposite direction, multiply by -1 to make it look like we are
            // moving the world instead of the camera 
            engine.camera.scroll(sf::Vector2f(-1 * event.get_mousedt()));
            return true;
        }
        return false;
    };
    cameractx.bind(Event{sf::Event::MouseMoved}, scroll);

    Event zoom{sf::Event::MouseWheelScrolled};
    cameractx.bind(zoom, [&](const Event & event){
        if (engine.inputm.is_button_pressed(sf::Mouse::Middle)) {
            return false;
        }
        constexpr float zoomfactor = 2.0f;
        if (event.get_scroll() < 0) {
            engine.camera.zoom(1/zoomfactor);
        }
        else {
            engine.camera.zoom(zoomfactor);
        }
        return true;
    });


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

    Entity entity = engine.entityf.get("test");

    entity.set_cell(cell_t{1, 1});

    delete timer;
    // Main loop ////////////////////////////////////////////////////////////////

    engine.run();
}
