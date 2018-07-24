#include <iostream>
#include "ui.hpp"
#include "timer.hpp"

Position UI::mouse_pos() {
    return Position{window->mapPixelToCoords(sf::Mouse::getPosition(*window))};
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
		switch (event.key.code) {
		    case sf::Keyboard::F5:
			cmds.push_back(CMD::Restart);
			break;
		    case sf::Keyboard::Z:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
			|| sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) {
			    cmds.push_back(CMD::Undo);
			}
			break;
		    case sf::Keyboard::Q:
		    case sf::Keyboard::Escape:
		    case sf::Keyboard::Return:
		    case sf::Keyboard::Space:
			cmds.push_back(CMD::Quit);
		    default:
			break;
		}
		continue;
	    case sf::Event::Closed:
		cmds.push_back(CMD::Quit);
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
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
	cmds.push_back(CMD::CreateTile);
    }
    else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
	cmds.push_back(CMD::DeleteTile);
    }
    else if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
	cmds.push_back(CMD::Scroll);
    }
    // accept one undo per x milliseconds
    /*static CASE::Timer timer{};
    if (timer.dt() >= 500) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
	|| sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) {
	    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
		cmds.push_back(CMD::Undo);
		timer.reset();
	    }
	}
	else {
	    timer.stop();
	}
    }*/
    return cmds;
}
