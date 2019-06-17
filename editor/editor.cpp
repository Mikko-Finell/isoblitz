#include "entitymenu.hpp"
#include "tilemenu.hpp"
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

class TileEdit {
    input::Context editctx;
    input::Context uictx;
    TileMenu menu;
    Tile tile;
    input::Manager & inputm;

    TileEdit & operator=(const TileEdit &) = delete;
    TileEdit(const TileEdit &) = delete;

public:
    ~TileEdit();
    TileEdit(Engine & engine);
};

int main(int argc, char * argv[]) {
    Engine & engine = StateManager::create("Editor");
    StateManager::create("Test");

    std::unique_ptr<TileEdit> tile_ed;
    /*
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

    //Entity * entity = nullptr;
    //entity = engine.entityf.get("unit4");
    //entity->animation = engine.animf.get("unit4");
    //engine.anims.add(entity->animation);
    //engine.wrender.add(entity->animation.sprite, "test");

    auto sprite = engine.spritef.create(engine.wrender, "enemy1", "move-left");
    sprite->set_position(sf::Vector2f{200, 200});
    auto anim = engine.animf.create(engine.wrender, "unit4");
    anim->sprite->set_position(sf::Vector2f{200, 300});
    auto entity = engine.entityf.create(engine.wrender, "unit4");
    entity->set_cell(Cell{50, 0});
    auto tile = engine.tilef.create(engine.wrender, 3);

    input::Event event{sf::Event::KeyPressed};
    event.set_key(sf::Keyboard::Space);
    auto globctx = engine.inputm.get_global_context();
    globctx->bind(event, [&](){
        engine.animm.destroy(anim);
        engine.spritem.destroy(sprite);
        engine.entitym.destroy(entity);
        engine.tilem.destroy(tile);
    });

    event.set_key(sf::Keyboard::S);
    globctx->bind(event, [&](){
        StateManager::run("Test");
        return true;
    });
    
    //engine.load(engine.map.filename());
    engine.run();


}

EntityEdit::~EntityEdit() {
    engine.inputm.remove_context(&editctx);
    engine.inputm.remove_context(&uictx);
    //delete entity;
}

EntityEdit::EntityEdit(Engine & engine) 
     : menu(engine.spritef, engine.uirender, engine.entityf, 
            /*menuwidth*/config::tilew, config::winw, /*columns*/1),
     engine(engine)
{
    /*
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
    */
}

TileEdit::~TileEdit() {
    inputm.remove_context(&editctx);
    inputm.remove_context(&uictx);
}

TileEdit::TileEdit(Engine & engine)
    : menu(engine.spritef, engine.uirender, engine.tilef, 
           /*menuwidth*/ config::tilew, config::winw, /*columns*/2),
     inputm(engine.inputm)
{
    /*
    engine.inputm.push_context(editctx);
    engine.inputm.push_context(uictx);
    tile = engine.tilef.get(1);
    menu.tile_selected.add_callback("set_tile_type", [&](tile_id_t type){
        tile = engine.tilef.get(type);
    });

    using namespace input;

    Event synctile{sf::Event::MouseMoved};
    editctx.bind(synctile, [&](const Event & event){
        auto pos = Position(event.get_mousepos());
        // note: All entities are given an offset, so that when we say 
        // sprite.set_position the actual x,y result is something that makes 
        // sense like the center of a tile or the feet of a unit. That is why 
        // this is required, it's like the mouse cursor's offset.
        pos.y += TILEH / 2;
        auto coord = coord_t(pos).to_grid();
        tile.set_coordinate(coord);

        if (engine.inputm.is_button_pressed(sf::Mouse::Left)) {
            engine.map.add_tile(tile.get_id(), coord);
            return true;
        }
        else if (engine.inputm.is_button_pressed(sf::Mouse::Right)) {
            engine.map.remove_tile(coord);
            return true;
        }
        return false;
    });

    Event edit_tile{sf::Event::MouseButtonPressed};
    edit_tile.set_button(sf::Mouse::Left);
    editctx.bind(edit_tile, [&](const Event & event){
        engine.map.add_tile(tile.get_id(), tile.get_coordinate());
        return true;
    });
    edit_tile.set_button(sf::Mouse::Right);
    editctx.bind(edit_tile, [&](const Event & event){
        engine.map.remove_tile(tile.get_coordinate());
        return true;
    });

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
    */
}
