#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "ui.hpp"
#include "cmd.hpp"
#include "map.hpp"
#include "brush.hpp"
#include "helper.hpp"

int main() {
    sf::RenderWindow window{sf::VideoMode{WINW, WINH}, "Bullet Editor"};
    sf::Texture spritesheet{};
    spritesheet.loadFromFile("sprites128x64.png");
    window.setKeyRepeatEnabled(false);

START:
    UI ui;
    ui.attach(window);

    Brush brush;
    brush.attach(ui);
    
    Map map;
    std::vector<sf::Vertex> vertices;

    while (window.isOpen()) {
	auto cmds = ui.handle_events();

	for (auto cmd : cmds) {
	    if (cmd == CMD::Restart) {
		goto START;
	    }
	    else if (cmd == CMD::Quit) {
		window.close();
		break;
	    }
	    else if (cmd == CMD::Scroll) {
		auto view = window.getView();
		auto dt = ui.mouse_dt();
		view.move(dt);
		window.setView(view);
	    }
	    else if (cmd == CMD::CreateTile) {
		map.create(brush.current_tile, brush.coordinate());
	    }
	    else if (cmd == CMD::DeleteTile) {
		map.remove(brush.coordinate());
	    }
	    else if (cmd == CMD::Undo) {
		map.undo();
	    }
	}

	vertices.clear();
	map.draw(vertices);

	//auto tmptile = Tile::from_position(ui.mouse_pos());
	//tmptile.draw(vertices);
	brush.draw(vertices);
        
	window.clear(sf::Color::White);
        window.draw(&vertices[0], vertices.size(), sf::Quads, &spritesheet);
	window.display();
    }
}
