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
#include "shell.hpp"

class Editor : public Listener {
    sf::RenderWindow window;
    sf::Texture spritesheet{};
    const std::string default_spritesheet{"sprites128x64.png"};
    std::vector<sf::Vertex> vertices;

public:
    void launch();
    void set_spritesheet(const std::string & filename);
    void recvevent(Event event) override;
};

int main() {
    Editor editor;
    editor.launch();
}

void Editor::launch() {

    window.create(sf::VideoMode{WINW, WINH}, "Bullet Editor");
    window.setKeyRepeatEnabled(false);
    set_spritesheet(default_spritesheet);

START:
    Shell shell;
    shell.launch();

    UI ui{window};
    Brush brush;
    Map map;

    shell.addlistener(this);
    shell.addlistener(&brush);
    ui.addlistener(&brush);
    ui.addlistener(&map);
    ui.addlistener(this);
    brush.addlistener(&map);

    while (window.isOpen()) {
	shell.emit_events();
	auto ui_events = ui.handle_events();

	for (auto event : ui_events) {
	    if (event == Event::Restart) {
		goto START;
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

void Editor::recvevent(Event event) {
    if (event == Event::Quit) {
	window.close();
    }
    else if (event == Event::ReloadSprites) {
	set_spritesheet(default_spritesheet);
    }
}

void Editor::set_spritesheet(const std::string & filename) {
    spritesheet.loadFromFile(filename);
}
