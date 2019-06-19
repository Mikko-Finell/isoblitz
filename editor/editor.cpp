#include "tileedit.hpp"
#include "entityedit.hpp"
#include "common/state.hpp"
#include <CASE/timer.hpp>
#include <iostream>
#include <memory>

int main(int argc, char * argv[]) {
    auto & engine = StateManager::create("Editor");

    /*
    auto sprite = engine.spritef.create(engine.wrender, "enemy1", "move-left");
    sprite.set_position(sf::Vector2f{200, 200}).set_layer(config::entity_layer);
    auto anim = engine.animf.create(engine.wrender, "enemy1");
    anim.set_sequence("move-down");
    anim.sprite.set_position(sf::Vector2f{200, 300}).set_layer(config::entity_layer);
    auto entity = engine.entityf.create(engine.wrender, "unit4");
    entity.set_coordinate(Coordinate{256, 0});
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

    event.set_key(sf::Keyboard::T);
    globctx->bind(event, [&](){
        auto & tileengine = StateManager::create("Tile");
        TileEdit tiledit{tileengine};
        tileengine.run();
    });
    event.set_key(sf::Keyboard::E);
    globctx->bind(event, [&](){
        auto & tileengine = StateManager::create("Entity");
        EntityEdit entityedit{tileengine};
        tileengine.run();
    });
    */

    std::unique_ptr<TileEdit> tileptr{nullptr};
    std::unique_ptr<EntityEdit> entityptr{nullptr};

    input::Event event{sf::Event::KeyPressed};
    event.set_key(sf::Keyboard::T);
    auto globctx = engine.inputm.get_global_context();
    globctx->bind(event, [&](){
        if (tileptr == nullptr) {
            tileptr.reset(new TileEdit{engine});
            entityptr.reset(nullptr);
        }
    });

    event.set_key(sf::Keyboard::E);
    globctx->bind(event, [&](){
        if (entityptr == nullptr) {
            tileptr.reset(nullptr);
            entityptr.reset(new EntityEdit{engine});
        }
    });

    tileptr.reset(new TileEdit{engine});
    engine.run();
}
