#include "ui.hpp"
#include "map.hpp"
#include "brush.hpp"
#include "shell.hpp"
#include "common/helper.hpp"
#include "common/observer.hpp"
#include "common/sprite.hpp"
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

class Editor : public Observer {
    sf::RenderWindow window;
    sf::Texture spritesheet;
    const std::string sprite_dir = "../sprites/";
    std::string spritesheet_filename = "sprites128x64.png";
    std::vector<sf::Vertex> vertices;
    sf::Color bgcolor = sf::Color::Black;

public:
    void launch();
    void on_set_bgcolor(const sf::Color & color);
};

int main() {
    Editor editor;
    editor.launch();
}

void Editor::launch() {
    window.create(sf::VideoMode{WINW, WINH}, "Bullet Editor");
    //window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(60);

    gfx::SpriteManager spritem;
    spritem.texture.loadFromFile(sprite_dir + spritesheet_filename);

    Shell shell;
    shell.launch();

    UI ui{window};
    Map map{spritem};
    Brush brush{map, spritem};
    // shell -> this
    shell.signal.quit.add_callback([this](){ window.close(); });
    shell.signal.set_bgcolor.add_observer(this, &Editor::on_set_bgcolor);
    // map -> this
    map.signal.map_loaded.add_callback([this](float w, float h){
        const sf::Vector2f v(w / 2, h / 2);
        auto u = logic_to_pixel(v);
        auto view = window.getView();
        view.setCenter(u.x, u.y);
        window.setView(view);
    });
    // ui -> map
    ui.signal.load.add_observer(map, &Map::on_load);
    ui.signal.save.add_observer(map, &Map::on_save);
    ui.signal.newmap.add_observer(map, &Map::on_new);
    ui.signal.setmapname.add_observer(map, &Map::on_setname);
    // ui -> brush
    ui.signal.paint.add_observer(brush, &Brush::on_paint);
    ui.signal.erase.add_observer(brush, &Brush::on_erase);
    ui.signal.update_mousepos.add_observer(brush, &Brush::on_update_mousepos);
    ui.signal.setblocked.add_observer(brush, &Brush::on_setblocked);
    ui.signal.paint.add_observer(brush, &Brush::on_paint);
    ui.signal.setsprite.add_observer(brush, &Brush::on_setsprite);
    // ui -> map
    ui.signal.undo.add_observer(map, [&map](){ map.undo(); });
    // shell -> brush
    shell.signal.set_blocked.add_observer(brush, &Brush::on_setblocked);
    shell.signal.set_sprite.add_observer(brush, &Brush::on_setsprite);
    shell.signal.paint.add_observer(brush, &Brush::on_paint_at);
    // shell -> map
    shell.signal.save.add_observer(map, &Map::on_save);
    shell.signal.load.add_observer(map, &Map::on_load);
    shell.signal.newmap.add_observer(map, &Map::on_new);
    shell.signal.set_mapname.add_observer(map, &Map::on_setname);

    while (window.isOpen()) {
	shell.emit_signals();
	ui.process_input();
	window.clear(bgcolor);
        spritem.draw(window);
	window.display();
    }
}

void Editor::on_set_bgcolor(const sf::Color & color) {
    bgcolor = color;
}
