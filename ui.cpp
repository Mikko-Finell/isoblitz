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

sf::Vector2f UI::mouse_dt() {
    return current_mouse_dt;
}

std::vector<Event> UI::handle_events() {
    std::vector<Event> events;
    sf::Event event;
    current_mouse_dt = sf::Vector2f{0,0};

    while (window->pollEvent(event)) {
	switch (event.type) {
	    case sf::Event::KeyPressed:
		switch (event.key.code) {
		    case sf::Keyboard::F5:
			events.push_back(Event::Restart);
			break;
		    case sf::Keyboard::F6:
			emit(Event::SetSpriteSheet);
			break;
		    case sf::Keyboard::Z:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
			|| sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) {
			    emit(Event::Undo);
			}
			break;
		    case sf::Keyboard::S:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
			|| sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) {
			    emit(Event::Save);
			}
			break;
		    case sf::Keyboard::L:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
			|| sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) {
			    emit(Event::Load);
			}
			break;
		    case sf::Keyboard::N:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
			|| sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) {
			    emit(Event::New);
			}
			break;
		    case sf::Keyboard::Q:
		    case sf::Keyboard::Escape:
		    case sf::Keyboard::Return:
		    case sf::Keyboard::Space:
			emit(Event::Quit);
		    default:
			break;
		}
		continue;
	    case sf::Event::Closed:
		emit(Event::Quit);
		continue;
	    case sf::Event::MouseButtonPressed:
		/*
		switch (event.mouseButton.button) {
		    case sf::Mouse::Left:
			break;
		    case sf::Mouse::Right:
			break;
		    default:
			break;
		}
		*/
		continue;
	    case sf::Event::MouseButtonReleased:
		/*
		switch (event.mouseButton.button) {
		    case sf::Mouse::Left:
			break;
		    case sf::Mouse::Right:
			break;
		    default:
			break;
		}
		*/
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
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            Event event{Event::Paint};
            emit(event);
        }
        else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            Event event{Event::Erase};
            emit(event);
        }
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
        events.push_back(Event::Scroll);
    }
    // update listeners on current mouse position
    Event mousepos{Event::MousePosition};
    mousepos.param = mouse_pos();
    emit(mousepos);

    return events;
}
