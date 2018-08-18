#include "map.hpp"
#include "shell.hpp"
#include "tilefactory.hpp"
#include "tilemenu.hpp"
#include "common/camera.hpp"
#include "common/util.hpp"
#include "common/input.hpp"
#include "common/sprite.hpp"
#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <getopt.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>

int main(int argc, char * argv[]) {
    sf::RenderWindow window;
    Camera camera{window};
    RenderSystem render;
    SpriteFactory spritef;
    Map map{render};
    Shell shell;
    input::Manager inputm{window};
    using namespace input;
    Context gctx;
    Context editctx;
    Context uictx;
    TileFactory tilef{render};

    // systems setup ////////////////////////////////////////////////////////////

    window.create(sf::VideoMode{WINW, WINH}, "Bullet Editor");
    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(60);
    util::center_window(window);
    render.load_texture("../sprites/sprites.png");
    sf::Color bgcolor = sf::Color::White;

    inputm.push_context(gctx);
    inputm.push_context(editctx);
    inputm.push_context(uictx);

    // getopt ///////////////////////////////////////////////////////////////////

    while (true) {
        static struct option long_options[] = {
            {"repl", no_argument, 0, 0},
            {"file", required_argument, 0, 'f'}
        };
        auto c = getopt_long(argc, argv, "f:0", long_options, NULL);
        if (c == -1) {
            break;
        }
        switch (c) {
            case 0:
                shell.launch();
                break;
            case 1:
            case 'f':
                map.load(optarg);
                break;
            default:
                break;
        }
    }

    // testing //////////////////////////////////////////////////////////////////

    //TileMenu tilemenu{spritef, render, tilef, 128, WINH, 2};


    // input mapping ////////////////////////////////////////////////////////////
    Tile brushtile = tilef.get(1);

    Event synctile{sf::Event::MouseMoved};
    editctx.bind(synctile, [&](const Event & event){
        if (inputm.is_button_pressed(sf::Mouse::Middle)) {
            // note: mousedt is the actual change, and so moving the camera by
            // that amount makes it look like the world is scrolling in the 
            // opposite direction, multiply by -1 to make it look like we are
            // moving the world instead of the camera 
            camera.scroll(sf::Vector2f(-1 * event.get_mousedt()));
            return true;
        }

        auto pos = Position(event.get_mousepos());
        // note: All entities are given an offset, so that when we say 
        // sprite.set_position the actual x,y result is something that makes 
        // sense like the center of a tile or the feet of a unit. That is why 
        // this is required, it's like the mouse cursor's offset.
        pos.y += TILEH / 2;
        auto coord = coord_t(pos).to_grid();
        brushtile.set_coordinate(coord);
        return true;
    });

    gctx.bind("quit", [&](){ window.close(); });

    Event event{sf::Event::Closed};
    gctx.bind(event, "quit");

    event.set_type(sf::Event::KeyPressed);
    event.set_key(sf::Keyboard::Q);
    gctx.bind(event, "quit");

    event.set_key(sf::Keyboard::S);
    editctx.bind(event, [&](){

    });

    event.set_key(sf::Keyboard::L);
    editctx.bind(event, [&](){

    });

    event.set_key(sf::Keyboard::N);
    editctx.bind(event, [&](){

    });

    input::Event zoom{sf::Event::MouseWheelScrolled};
    uictx.bind(zoom, [&](const input::Event & event){
        constexpr float zoomfactor = 2.0f;
        if (event.get_scroll() > 0) {
            camera.zoom(zoomfactor);
        }
        else {
            camera.zoom(1/zoomfactor);
        }
        return true;
    });

    auto tilemenu_hover = [&](const Event & event){
        const auto pos = event.get_mousepos();
        if (tilemenu.contains(pos)) {
            tilemenu.update_mousepos(event.get_mousepos());
            return true;
        }
        return false;
    };
    uictx.bind(input::Event{sf::Event::MouseMoved}, tilemenu_hover);

    auto tilemenu_click = [&](const Event & event){
        const auto pos = event.get_mousepos();
        return tilemenu.click(event.get_mousepos());
    };
    Event clickevnt{sf::Event::MouseButtonReleased};
    clickevnt.set_button(sf::Mouse::Left);
    uictx.bind(clickevnt, tilemenu_click);

    // signal coupling //////////////////////////////////////////////////////////

    shell.signal.quit.add_callback([&](){ window.close(); });
    shell.signal.set_bgcolor.add_callback([&](auto & c){ bgcolor = c; });

    // main loop ////////////////////////////////////////////////////////////////

    while (window.isOpen()) {
	shell.emit_signals();
        inputm.poll_sfevents();

	window.clear(bgcolor);
        render.draw(window);
        ui_render.draw(window);
	window.display();
    }
}
