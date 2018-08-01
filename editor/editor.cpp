#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "common/helper.hpp"
#include "common/observer.hpp"
#include "ui.hpp"
#include "map.hpp"
#include "brush.hpp"
#include "shell.hpp"

class Editor : public Observer {
    sf::RenderWindow window;
    sf::Texture spritesheet;
    const std::string sprite_dir = "../sprites/";
    std::string spritesheet_filename = "sprites128x64.png";
    std::vector<sf::Vertex> vertices;

public:
    void launch();
    void set_spritesheet(const std::string & filename);
};

int main() {
    Editor editor;
    editor.launch();
}

void Editor::launch() {
    window.create(sf::VideoMode{WINW, WINH}, "Bullet Editor");
    window.setKeyRepeatEnabled(false);
    set_spritesheet(spritesheet_filename);

    Shell shell;
    shell.launch();

    UI ui{window};
    Map map;
    Brush brush{map};

    // new eventhandling subscriptions
    ui.events.quit.add_callback([this](){ window.close(); });
    // ui -> map
    ui.events.load.add_observer(map, &Map::on_load);
    ui.events.save.add_observer(map, &Map::on_save);
    ui.events.newmap.add_observer(map, &Map::on_new);
    ui.events.setmapname.add_observer(map, &Map::on_setname);
    // ui -> brush
    ui.events.paint.add_observer(brush, &Brush::on_paint);
    ui.events.erase.add_observer(brush, &Brush::on_erase);
    ui.events.update_mousepos.add_observer(brush, &Brush::on_update_mousepos);
    ui.events.setblocked.add_observer(brush, &Brush::on_setblocked);
    ui.events.paint.add_observer(brush, &Brush::on_paint);
    ui.events.setsprite.add_observer(brush, &Brush::on_setsprite);
    // ui -> map
    ui.events.undo.add_observer(map, [&map](){ map.undo(); });
    // shell -> this
    shell.events.quit.add_callback([this](){ window.close(); });
    // shell -> brush
    shell.events.set_blocked.add_observer(brush, &Brush::on_setblocked);
    shell.events.set_sprite.add_observer(brush, &Brush::on_setsprite);
    // shell -> map
    shell.events.save.add_observer(map, &Map::on_save);
    shell.events.load.add_observer(map, &Map::on_load);
    shell.events.newmap.add_observer(map, &Map::on_new);
    shell.events.set_mapname.add_observer(map, &Map::on_setname);

    while (window.isOpen()) {
	shell.emit_events();
	ui.process_input();

	vertices.clear();
	map.draw(vertices);
	brush.draw(vertices);
        
	window.clear(sf::Color::White);
        window.draw(&vertices[0], vertices.size(), sf::Quads, &spritesheet);
	window.display();
    }
}

void Editor::set_spritesheet(const std::string & filename) {
    spritesheet.loadFromFile(sprite_dir + filename);
}
