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
		    case sf::Keyboard::Z:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
			|| sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) {
			    events.push_back(Event::Undo);
			}
			break;
		    case sf::Keyboard::Q:
		    case sf::Keyboard::Escape:
		    case sf::Keyboard::Return:
		    case sf::Keyboard::Space:
			events.push_back(Event::Quit);
		    default:
			break;
		}
		continue;
	    case sf::Event::Closed:
		events.push_back(Event::Quit);
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
	events.push_back(Event::CreateTile);
    }
    else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
	events.push_back(Event::DeleteTile);
    }
    else if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
	events.push_back(Event::Scroll);
    }
    // accept one undo per x milliseconds
    /*static CASE::Timer timer{};
    if (timer.dt() >= 500) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
	|| sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) {
	    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
		events.push_back(Event::Undo);
		timer.reset();
	    }
	}
	else {
	    timer.stop();
	}
    }*/
    return events;
}
