#include <iostream>
#include "ui.hpp"

UI::UI(sf::RenderWindow & w) : window(&w) {
}

Position UI::mouse_pos() {
    auto sfvec = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
    return Position{sfvec.x, sfvec.y};
}

bool UI::is_mouse_pressed() {
    return mouse_pressed;
}

//sf::Vector2f UI::mouse_dt() {
    //return current_mouse_dt;
//}

void UI::process_input() {
    sf::Event event;
    current_mouse_dt = sf::Vector2f{0,0};

    while (window->pollEvent(event)) {
	switch (event.type) {
	    case sf::Event::KeyPressed:
		switch (event.key.code) {
		    case sf::Keyboard::F6:
                        signal.setspritesheet("");
			break;
		    case sf::Keyboard::Z:
                        if (event.key.control) {
                            signal.undo();
			}
			break;
		    case sf::Keyboard::S:
                        if (event.key.control) {
                            signal.save("");
			}
			break;
		    case sf::Keyboard::L:
                        if (event.key.control) {
                            signal.load("");
			}
			break;
		    case sf::Keyboard::N:
                        if (event.key.control) {
                            signal.newmap("");
			}
			break;
		    case sf::Keyboard::Q:
		    case sf::Keyboard::Escape:
		    case sf::Keyboard::Return:
		    case sf::Keyboard::Space:
                        signal.quit();
		    default:
			break;
		}
		continue;
	    case sf::Event::Closed:
                signal.quit();
		continue;
	    case sf::Event::MouseButtonPressed:
		continue;
	    case sf::Event::MouseButtonReleased:
		continue;
	    case sf::Event::MouseMoved:
		current_mouse_dt.x += prev_mouse_pos.x - event.mouseMove.x;
		current_mouse_dt.y += prev_mouse_pos.y - event.mouseMove.y;
		prev_mouse_pos.x = event.mouseMove.x;
		prev_mouse_pos.y = event.mouseMove.y;
		continue;
	default:
		continue;
	}
    }

    auto screen_mouse_pos = sf::Mouse::getPosition();
    auto wpos = window->getPosition();
    auto wsize = window->getSize();
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
        auto view = window->getView();
        view.move(current_mouse_dt);
        window->setView(view);
    }
    // update listeners on current mouse position
    signal.update_mousepos(mouse_pos());
}
