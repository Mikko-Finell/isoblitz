#include "ui.hpp"
#include "common/input.hpp"
#include <iostream>

static input::Manager inputm;

UI::UI(sf::RenderWindow & w) : window(w), camera(w) {
    inputm.set_window(w);


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
