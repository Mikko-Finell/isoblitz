#include "minimap.hpp"

void Minimap::update(float dt) {
    for (auto entity : needs_update) {
        Sprite & sprite = entity_sprites[entity];
        // update sprite position
    }
    needs_update.clear();
}

void Minimap::add_entity(Entity & entity) {
    //entity_sprites[&entity] = spritef.create(engine.uirender, "minimap-entity");
    //needs_update.push_back(&entity);
}

void Minimap::remove_entity(Entity & entity) {
    if (entity_sprites.erase(&entity) == 1) {
        for (auto itr = needs_update.begin(); itr != needs_update.end(); itr++) {
            if (*itr == &entity) {
                needs_update.erase(itr);
                return;
            }
        }
    }
}

Minimap::Minimap(Engine & engine) {
    engine.signals.update.add_callback([&](float dt){
        system_update(dt);
    });
    engine.entitym.signals.entity_spawned.add_observer(this, [&](Entity & entity){
        system_add_entity(entity);
    });
    engine.moves.signals.entity_move.add_observer(this, [&](Entity & entity, auto &&){
        needs_update.push_back(&entity);
    });

    sf::Image img; img.loadFromFile("../sprites/tmp.png");
    auto pixels = img.getPixelsPtr();
    engine.sfml.texture.update(pixels, config::minimap_width, config::minimap_height, 0, 0);

    worldsprite = engine.spritef.create(engine.uirender, "minimap-world");
    worldsprite.set_position(0, config::winh - config::minimap_height);
}
