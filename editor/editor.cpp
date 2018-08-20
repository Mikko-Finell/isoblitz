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
#include <memory>

class TileEdit {
    input::Context tilectx;
    input::Context uictx;
    TileMenu tilemenu;
    Brush brush;
    input::Manager & inputm;

    TileEdit & operator=(const TileEdit &) = delete;
    TileEdit(const TileEdit &) = delete;

public:
    ~TileEdit();
    TileEdit(Engine & engine);
};

class EntityEdit {
    EntityEdit & operator=(const EntityEdit &) = delete;
    EntityEdit(const EntityEdit &) = delete;

public:
    ~EntityEdit() { }
    EntityEdit(Engine & engine) { }
};

int main(int argc, char * argv[]) {
    Engine engine;
    std::unique_ptr<TileEdit> tile_ed;
    std::unique_ptr<EntityEdit> entity_ed;

    auto globctx = engine.inputm.get_global_context();

    input::Event event{sf::Event::KeyPressed};
    event.set_key(sf::Keyboard::T);
    globctx->bind(event, [&](){
        entity_ed.reset(nullptr);
        tile_ed.reset(new TileEdit{engine});
    });

    event.set_key(sf::Keyboard::E);
    globctx->bind(event, [&](){
        tile_ed.reset(nullptr);
        entity_ed.reset(new EntityEdit{engine});
    });

    // main loop ////////////////////////////////////////////////////////////////

    engine.run();
}

TileEdit::~TileEdit() {
    inputm.remove_context(&tilectx);
    inputm.remove_context(&uictx);
}

TileEdit::TileEdit(Engine & engine)
    : tilemenu(engine.spritef, engine.uirender, engine.tilef, 128, WINH, 2),
     brush(engine.tilef, engine.map),
     inputm(engine.inputm)
{
    engine.inputm.push_context(tilectx);
    engine.inputm.push_context(uictx);
    tilemenu.tile_selected.add_observer(brush, &Brush::on_tile_selected);

    using namespace input;

    Event synctile{sf::Event::MouseMoved};
    tilectx.bind(synctile, [&](const Event & event){
        auto pos = Position(event.get_mousepos());
        // note: All entities are given an offset, so that when we say 
        // sprite.set_position the actual x,y result is something that makes 
        // sense like the center of a tile or the feet of a unit. That is why 
        // this is required, it's like the mouse cursor's offset.
        pos.y += TILEH / 2;
        auto coord = coord_t(pos).to_grid();
        brush.set_coordinate(coord);

        if (engine.inputm.is_button_pressed(sf::Mouse::Left)) {
            brush.paint();
            return true;
        }
        else if (engine.inputm.is_button_pressed(sf::Mouse::Right)) {
            brush.erase();
            return true;
        }
        return false;
    });

    Event edit_tile{sf::Event::MouseButtonPressed};
    edit_tile.set_button(sf::Mouse::Left);
    tilectx.bind(edit_tile, [&](const Event & event){
        brush.paint();
        return true;
    });
    edit_tile.set_button(sf::Mouse::Right);
    tilectx.bind(edit_tile, [&](const Event & event){
        brush.erase();
        return true;
    });

    uictx.bind(input::Event{sf::Event::MouseMoved}, [&](const Event & event){
        auto p = Position(engine.window.mapCoordsToPixel(event.get_mousepos()));
        tilemenu.update_mousepos(p);
        return tilemenu.contains(p);
    });

    Event clickevnt{sf::Event::MouseButtonPressed};
    clickevnt.set_button(sf::Mouse::Left);
    uictx.bind(clickevnt, [&](const Event & event){
        auto p = Position(engine.window.mapCoordsToPixel(event.get_mousepos()));
        return tilemenu.try_click(p);
    });
}
