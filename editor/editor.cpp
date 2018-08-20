#include "brush.hpp"
#include "shell.hpp"
#include "tilemenu.hpp"
#include "common/engine.hpp"
#include "common/util.hpp"
#include <CASE/timer.hpp>
#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <getopt.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>

int main(int argc, char * argv[]) {
    auto timer = new CASE::ScopeTimer{"Create systems"};

    Engine engine;
    input::Context editctx;
    input::Context uictx;
    TileMenu tilemenu{engine.spritef, engine.uirender, engine.tilef,128,WINH,2};
    Brush brush{engine.tilef, engine.map};

    delete timer;
    // systems setup ////////////////////////////////////////////////////////////
    timer = new CASE::ScopeTimer{"Init systems"};

    engine.inputm.push_context(editctx);
    engine.inputm.push_context(uictx);

    tilemenu.tile_selected.add_observer(brush, &Brush::on_tile_selected);

    delete timer;
    // getopt ///////////////////////////////////////////////////////////////////

    /*
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
                //map.load(optarg);
                break;
            default:
                break;
        }
    }
    */

    // testing //////////////////////////////////////////////////////////////////

    // input mapping ////////////////////////////////////////////////////////////
    timer = new CASE::ScopeTimer{"Input mapping"};

    using namespace input;
    Event synctile{sf::Event::MouseMoved};
    editctx.bind(synctile, [&](const Event & event){
        bool b = false;
        if (engine.inputm.is_button_pressed(sf::Mouse::Left)) {
            brush.paint();
            b = true;
        }
        else if (engine.inputm.is_button_pressed(sf::Mouse::Right)) {
            brush.erase();
            b = true;
        }

        auto pos = Position(event.get_mousepos());
        // note: All entities are given an offset, so that when we say 
        // sprite.set_position the actual x,y result is something that makes 
        // sense like the center of a tile or the feet of a unit. That is why 
        // this is required, it's like the mouse cursor's offset.
        pos.y += TILEH / 2;
        auto coord = coord_t(pos).to_grid();
        brush.set_coordinate(coord);
        return b;
    });

    Event edit_tile{sf::Event::MouseButtonPressed};
    edit_tile.set_button(sf::Mouse::Left);
    auto brush_paint = [&](const Event & event){
        brush.paint();
        return true;
    };
    editctx.bind(edit_tile, brush_paint);
    edit_tile.set_button(sf::Mouse::Right);
    auto brush_erase = [&](const Event & event){
        brush.erase();
        return true;
    };
    editctx.bind(edit_tile, brush_erase);

    auto tilemenu_hover = [&](const Event & event){
        auto p = Position(engine.window.mapCoordsToPixel(event.get_mousepos()));
        tilemenu.update_mousepos(p);
        return tilemenu.contains(p);
    };
    uictx.bind(input::Event{sf::Event::MouseMoved}, tilemenu_hover);

    auto tilemenu_click = [&](const Event & event){
        auto p = Position(engine.window.mapCoordsToPixel(event.get_mousepos()));
        return tilemenu.try_click(p);
    };
    Event clickevnt{sf::Event::MouseButtonPressed};
    clickevnt.set_button(sf::Mouse::Left);
    uictx.bind(clickevnt, tilemenu_click);

    delete timer;
    // signal coupling //////////////////////////////////////////////////////////

    //shell.signal.quit.add_callback([&](){ window.close(); });
    //shell.signal.set_bgcolor.add_callback([&](auto & c){ bgcolor = c; });

    // main loop ////////////////////////////////////////////////////////////////

    //shell.emit_signals();
    engine.run();
}
