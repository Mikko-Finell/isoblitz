#include "ui.hpp"
#include <iostream>

sf::Vector2f UI::mouse_pos() {
    return window->mapPixelToCoords(sf::Mouse::getPosition(*window));
}

bool UI::is_mouse_pressed() {
    return mouse_pressed;
}

sf::Vector2f UI::mouse_dt() {
    return current_mouse_dt;
}

void UI::attach(sf::RenderWindow & w) {
    window = &w;
}

std::vector<CMD> UI::handle_events() {
    std::vector<CMD> cmds;
    sf::Event event;
    current_mouse_dt = sf::Vector2f{0,0};

    while (window->pollEvent(event)) {
	switch (event.type) {
	    case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::F5) {
		    cmds.push_back(CMD::Restart);
		}
		else {
		    cmds.push_back(CMD::Quit);
		}
		continue;
	    case sf::Event::Closed:
		cmds.push_back(CMD::Quit);
		continue;
	    case sf::Event::MouseButtonPressed:
		if (event.mouseButton.button != sf::Mouse::Middle) {
		    mouse_pressed = true;
		}
		continue;
	    case sf::Event::MouseButtonReleased:
		if (event.mouseButton.button != sf::Mouse::Middle) {
		    mouse_pressed = false;
		}
		continue;
	    case sf::Event::MouseMoved:
		if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
		    auto dx = prev_mouse_pos.x - event.mouseMove.x;
		    auto dy = prev_mouse_pos.y - event.mouseMove.y;
		    current_mouse_dt += sf::Vector2f{dx, dy};

		    if (current_mouse_dt == sf::Vector2f{0,0}) {
			cmds.push_back(CMD::Scroll);
		    }
		}
		prev_mouse_pos.x = event.mouseMove.x;
		prev_mouse_pos.y = event.mouseMove.y;
		continue;
	default:
		continue;
	}
    }
    return cmds;
}
