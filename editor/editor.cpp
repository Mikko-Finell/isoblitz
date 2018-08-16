#include "ui.hpp"
#include "map.hpp"
#include "brush.hpp"
#include "shell.hpp"
#include "common/util.hpp"
#include "common/observer.hpp"
#include "common/sprite.hpp"
#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <getopt.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Editor : public Observer {
    sf::RenderWindow window;
    sf::Texture spritesheet;
    const std::string sprite_dir = "../sprites/";
    std::string spritesheet_filename = "sprites.png";
    std::vector<sf::Vertex> vertices;
    sf::Color bgcolor = sf::Color::White;

public:
    Shell shell;

    void launch(const std::string & mapname);
    void on_set_bgcolor(const sf::Color & color);
};

int main(int argc, char * argv[]) {
    std::string mapname = "tmp";
    Editor editor;
    while (true) {
        static struct option long_options[] = {
            //{"repl", no_argument, 0, 0},
            {"file", required_argument, 0, 'f'}
        };
        auto c = getopt_long(argc, argv, "f:0", long_options, NULL);
        if (c == -1) {
            break;
        }
        switch (c) {
            case 0:
                //editor.shell.launch();
                break;
            case 1:
            case 'f':
                mapname = optarg;
                break;
            default:
                break;
        }
    }
    editor.shell.launch();
    editor.launch(mapname);
}

void Editor::launch(const std::string & mapname) {
    window.create(sf::VideoMode{WINW, WINH}, "Bullet Editor");
    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(60);
    util::center_window(window);

    SpriteManager spritem;
    spritem.load_texture(sprite_dir + spritesheet_filename);

    UI ui{window};
    Map map{spritem};
    Brush brush{map, spritem};
    // shell -> this
    shell.signal.quit.add_callback([this](){ window.close(); });
    shell.signal.set_bgcolor.add_observer(this, &Editor::on_set_bgcolor);
    // map -> this
    map.signal.map_loaded.add_callback([this](float w, float h){
        const sf::Vector2f v(w / 2, h / 2);
        auto u = util::to_pixel(v);
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
    ui.signal.toggle_snap.add_observer(brush, &Brush::toggle_snap);
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

    //map.on_load(mapname);

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
