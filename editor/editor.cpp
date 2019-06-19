#include "entitymenu.hpp"
#include "tilemenu.hpp"
#include "tilecursor.hpp"
#include "tilebrush.hpp"
#include "common/engine.hpp"
#include "common/util.hpp"
#include "common/state.hpp"
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

/*
class EntityEdit {
    input::Context editctx;
    input::Context uictx;
    EntityMenu menu;
    Entity * entity = nullptr;
    Engine & engine;

    EntityEdit & operator=(const EntityEdit &) = delete;
    EntityEdit(const EntityEdit &) = delete;

public:
    ~EntityEdit();
    EntityEdit(Engine & engine);
};
*/

class TileEdit {
    input::Context cursor_ctx;
    input::Context menu_ctx;
    input::Context brush_ctx;
    TileMenu menu;
    TileCursor cursor;
    TileBrush brush;

public:
    ~TileEdit();
    TileEdit(Engine & engine);
};

int main(int argc, char * argv[]) {
    auto & engine = StateManager::create("Editor");

    TileEdit tiledit{engine};

    /*
    std::unique_ptr<TileEdit> tile_ed;
    std::unique_ptr<EntityEdit> entity_ed{new EntityEdit{engine}};
    auto globctx = engine.inputm.get_global_context();
    input::Event event{sf::Event::KeyPressed};
    event.set_key(sf::Keyboard::E);
    globctx->bind(event, [&](){
        //tile_ed.reset(nullptr);
        //entity_ed.reset(new EntityEdit{engine});
    });
    event.set_key(sf::Keyboard::T);
    globctx->bind(event, [&](){
        entity_ed.reset(nullptr);
        //tile_ed.reset(new TileEdit{engine});
    });
    event.set_key(sf::Keyboard::N);
    event.set_mod(input::Mod::CTRL, true);
    globctx->bind(event, [&](){
        engine.reset();
        if (entity_ed) {
            entity_ed.reset(new EntityEdit{engine});
        }
        else {
            tile_ed.reset(new TileEdit{engine});
        }
    });

    event.set_key(sf::Keyboard::S);
    event.set_mod(input::Mod::CTRL, true);
    globctx->bind(event, [&](){
        engine.save(engine.map.filename());
        return true;
    });
    */
    auto sprite = engine.spritef.create(engine.wrender, "enemy1", "move-left");
    sprite.set_position(sf::Vector2f{200, 200}).set_layer(config::entity_layer);
    auto anim = engine.animf.create(engine.wrender, "enemy1");
    anim.set_sequence("move-down");
    anim.sprite.set_position(sf::Vector2f{200, 300}).set_layer(config::entity_layer);
    auto entity = engine.entityf.create(engine.wrender, "unit4");
    entity.set_cell(Cell{50, 0});
    entity.animation.set_sequence("die");
    auto & tile = engine.tilef.create(engine.wrender, 3);

    input::Event event{sf::Event::KeyPressed};
    event.set_key(sf::Keyboard::Space);
    auto globctx = engine.inputm.get_global_context();
    globctx->bind(event, [&](){
        anim.clear();
        sprite.clear();
        entity.clear();
        engine.tilem.destroy(tile);
    });

    event.set_key(sf::Keyboard::Return);
    globctx->bind(event, [&](){
        auto & tmp = StateManager::create("test");
        std::unique_ptr<input::Context> ctx{new input::Context{}};
        tmp.inputm.push_context(ctx.get());
        input::Event testevent{sf::Event::KeyPressed};
        testevent.set_key(sf::Keyboard::Return);
        ctx->bind(testevent, [&]() {
            ctx.reset(nullptr);
        });
        auto tile = tmp.tilef.create(tmp.wrender, 5);
        StateManager::run("test");
    });

    engine.run();
}

/*
EntityEdit::~EntityEdit() {
    engine.inputm.remove_context(&editctx);
    engine.inputm.remove_context(&uictx);
    //delete entity;
}

EntityEdit::EntityEdit(Engine & engine) 
     : menu(engine.spritef, engine.uirender, engine.entityf, 
            config::tilew,  // menu width
            config::winw, 
            1 // columns
       ),
     engine(engine)
{
    engine.inputm.push_context(editctx);
    engine.inputm.push_context(uictx);

    auto set_type = [&](type_id_t type){
        if (entity) {
            delete entity;
            entity = nullptr;
        }
        entity = engine.entityf.get(type);
    };
    menu.entity_selected.add_callback("set_type", set_type);
    //set_type("unit4");

    using namespace input;

    Event syncbrush{sf::Event::MouseMoved};
    editctx.bind(syncbrush, [&](const Event & event){
        auto pos = Position(event.get_mousepos());
        // note: All entities are given an offset, so that when we say 
        // sprite.set_position the actual x,y result is something that makes 
        // sense like the center of a tile or the feet of a unit. That is why 
        // this is required, it's like the mouse cursor's offset.
        pos.y += CELLH / 2;

        Coordinate<CELLW, CELLH> coord{pos};
        coord = coord.to_grid();
        entity->set_cell(coord);

        return false;
    });

    Event edit_entity{sf::Event::MouseButtonPressed};
    edit_entity.set_button(sf::Mouse::Left);
    editctx.bind(edit_entity, [&](const Event & event){
        auto e = engine.entityf.get(entity->get_type());
        e->set_cell(entity->get_cell());
        engine.entitym.add_entity(e);

        return true;
    });

    //edit_entity.set_button(sf::Mouse::Right);
    //editctx.bind(edit_entity, [&](const Event & event){
        //return true;
    //});

    uictx.bind(input::Event{sf::Event::MouseMoved}, [&](const Event & event){
        auto p = Position(engine.window.mapCoordsToPixel(event.get_mousepos()));
        menu.update_mousepos(p);
        return menu.contains(p);
    });

    Event clickevnt{sf::Event::MouseButtonPressed};
    clickevnt.set_button(sf::Mouse::Left);
    uictx.bind(clickevnt, [&](const Event & event){
        auto p = Position(engine.window.mapCoordsToPixel(event.get_mousepos()));
        if (menu.contains(p)) {
            menu.try_click(p);
            return true;
        }
        else {
            return false;
        }
    });
}
*/
TileEdit::~TileEdit() {
}

TileEdit::TileEdit(Engine & engine)
    : menu(engine), cursor(engine), brush(engine)
{
    using namespace input;
    engine.inputm.push_context(cursor_ctx);
    engine.inputm.push_context(brush_ctx);
    engine.inputm.push_context(menu_ctx);

    /////////////////////////////////////////////////////////////////////////////// MENU SETUP

    menu_ctx.bind(Event{sf::Event::MouseMoved}, [&](const Event & event){
        auto p = event.get_mousepos_pixel();
        menu.update_mousepos(p);
        return menu.contains(p);
    });

    Event clickevnt{sf::Event::MouseButtonPressed};
    clickevnt.set_button(sf::Mouse::Left);
    menu_ctx.bind(clickevnt, [&](const Event & event){
        auto p = event.get_mousepos_pixel();
        return menu.click(p);
    });

    menu.tile_selected.add_callback("select", [&](Tile::ID id){
        cursor.set_tile_type(id);
    });

    /////////////////////////////////////////////////////////////////////////////// CURSOR SETUP

    Event synctile{sf::Event::MouseMoved};
    cursor_ctx.bind(synctile, [&](const Event & event){
        auto pos = event.get_mousepos_logic();
        cursor.update_mousepos(pos);
        return engine.inputm.is_button_pressed(sf::Mouse::Middle) == false;
    });

    /////////////////////////////////////////////////////////////////////////////// BRUSH SETUP

    Event edit_tile{sf::Event::MouseButtonPressed};
    edit_tile.set_button(sf::Mouse::Left);
    brush_ctx.bind(edit_tile, [&](const Event & event){
        brush.add_tile(cursor.get_id(), cursor.get_coordinate());
        return true;
    });
    edit_tile.set_button(sf::Mouse::Right);
    brush_ctx.bind(edit_tile, [&](const Event & event){
        brush.remove_tile(cursor.get_coordinate());
        return true;
    });

    Event paint{sf::Event::MouseMoved};
    brush_ctx.bind(paint, [&](const Event & event){
        if (engine.inputm.is_button_pressed(sf::Mouse::Left)) {
            brush.add_tile(cursor.get_id(), cursor.get_coordinate());
        }
        else if (engine.inputm.is_button_pressed(sf::Mouse::Right)) {
            brush.remove_tile(cursor.get_coordinate());
        }
        return false;
    });
}
