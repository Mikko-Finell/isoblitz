#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "common/helper.hpp"
#include "event.hpp"
#include "ui.hpp"
#include "map.hpp"
#include "brush.hpp"
#include "shell.hpp"

class Editor : public Listener {
    sf::RenderWindow window;
    sf::Texture spritesheet;
    const std::string sprite_dir = "../sprites/";
    std::string spritesheet_filename = "sprites128x64.png";
    std::vector<sf::Vertex> vertices;
    Map map;

public:
    void launch();
    void set_spritesheet(const std::string & filename);
    void recvevent(const Event & event) override;
};

int main() {
    Editor editor;
    editor.launch();
}

void Editor::launch() {
    window.create(sf::VideoMode{WINW, WINH}, "Bullet Editor");
    window.setKeyRepeatEnabled(false);
    set_spritesheet(spritesheet_filename);
    std::cout << "Texture max size: " << sf::Texture::getMaximumSize() << std::endl;

    Shell shell;
    shell.launch();

    UI ui{window};
    Brush brush;

    shell.addlistener(this);
    shell.addlistener(&brush);
    shell.addlistener(&map);
    ui.addlistener(&brush);
    ui.addlistener(&map);
    ui.addlistener(this);
    brush.addlistener(&map);

    while (window.isOpen()) {
	shell.emit_events();
	auto ui_events = ui.handle_events();

	for (const Event & event : ui_events) {
            if (event == Event::Quit) {
                recvevent(event);
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

void Editor::recvevent(const Event & event) {
    if (event == Event::Quit) {
	window.close();
    }
    else if (event == Event::SetSpriteSheet) {
        if (auto filename = std::get_if<std::string>(&event.param)) {
            spritesheet_filename = *filename;
        }
	set_spritesheet(spritesheet_filename);
    }
}

void Editor::set_spritesheet(const std::string & filename) {
    spritesheet.loadFromFile(sprite_dir + filename);
}
