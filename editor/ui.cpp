#include "ui.hpp"
#include "common/input.hpp"
#include <iostream>

static input::Manager inputm;

UI::UI(sf::RenderWindow & w) : window(w), camera(w) {
    inputm.set_window(w);

    using namespace input;
    static Context gctx;

    inputm.push_context(gctx);
    gctx.bind("quit", [&](){ w.close(); });
    Event event{sf::Event::Closed};
    gctx.bind(event, "quit");
    event.set_type(sf::Event::KeyPressed);
    event.set_key(sf::Keyboard::Q);
    gctx.bind(event, "quit");

    static Context editctx;
    inputm.push_context(editctx);
    event.set_mod(Mod::CTRL, true);
    event.set_key(sf::Keyboard::Z);
    editctx.bind(event, [&](){ signal.undo(); });
    event.set_key(sf::Keyboard::S);
    editctx.bind(event, [&](){
        signal.save("");
    });
    event.set_key(sf::Keyboard::L);
    editctx.bind(event, [&](){
        signal.load("");
    });
    event.set_key(sf::Keyboard::N);
    editctx.bind(event, [&](){ signal.newmap(""); });

    input::Event zoom{sf::Event::MouseWheelScrolled};
    gctx.bind(zoom, [&](const input::Event & event){
        constexpr float zoomfactor = 2.0f;
        //if (inputm.is_key_pressed(sf::Keyboard::LControl)) {
            if (event.get_scroll() < 0) {
                camera.zoom(zoomfactor);
            }
            else {
                camera.zoom(1/zoomfactor);
            }
            return true;
        //}
        //return false;
    });

    input::Event snap{sf::Event::KeyPressed};
    snap.set_key(sf::Keyboard::G);
    editctx.bind(snap, [&](){ signal.toggle_snap(); });
}

sf::Vector2f UI::mouse_pos() {
    auto sfvec = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    return sf::Vector2f{sfvec.x, sfvec.y};
}

bool UI::is_mouse_pressed() {
    return mouse_pressed;
}

void UI::process_input() {
    sf::Event sfevent;
    current_mouse_dt = sf::Vector2f{0,0};

    bool moved = false;
    while (window.pollEvent(sfevent)) {
	switch (sfevent.type) {
	    case sf::Event::MouseButtonPressed:
		continue;
	    case sf::Event::MouseButtonReleased:
		continue;
	    case sf::Event::MouseMoved:
		current_mouse_dt.x += prev_mouse_pos.x - sfevent.mouseMove.x;
		current_mouse_dt.y += prev_mouse_pos.y - sfevent.mouseMove.y;
		prev_mouse_pos.x = sfevent.mouseMove.x;
		prev_mouse_pos.y = sfevent.mouseMove.y;
                moved = true;
		continue;
	default:
                inputm.process_event(sfevent);
		continue;
	}
    }

    auto screen_mouse_pos = sf::Mouse::getPosition();
    auto wpos = window.getPosition();
    auto wsize = window.getSize();
    sf::IntRect wrect(wpos, sf::Vector2i(wsize));

    if (wrect.contains(screen_mouse_pos)) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)) {
            // do nothing
        }
        else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            signal.paint();
        }
        else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            signal.erase();
        }
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
        camera.scroll(current_mouse_dt);
    }
    // update listeners on current mouse position
    if (moved) {
        signal.update_mousepos(mouse_pos());
    }
}
