#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "ui.hpp"
#include "event.hpp"
#include "map.hpp"
#include "brush.hpp"
#include "helper.hpp"

int main() {
    sf::RenderWindow window{sf::VideoMode{WINW, WINH}, "Bullet Editor"};
    sf::Texture spritesheet{};
    spritesheet.loadFromFile("sprites128x64.png");
    window.setKeyRepeatEnabled(false);

START:
    UI ui{window};
    Brush brush;
    Map map;

    ui.addlistener(&brush);
    brush.addlistener(&map);
    ui.addlistener(&map);

    std::vector<sf::Vertex> vertices;

    while (window.isOpen()) {
	auto events = ui.handle_events();

	for (auto event : events) {
	    if (event == Event::Restart) {
		goto START;
	    }
	    else if (event == Event::Quit) {
		window.close();
		break;
	    }
	    else if (event == Event::Scroll) {
		auto view = window.getView();
		auto dt = ui.mouse_dt();
		view.move(dt);
		window.setView(view);
	    }
	}

	vertices.clear();
	map.draw(vertices);
	brush.draw(vertices);
        
	window.clear(sf::Color::White);
        window.draw(&vertices[0], vertices.size(), sf::Quads, &spritesheet);
	window.display();
    }
}
