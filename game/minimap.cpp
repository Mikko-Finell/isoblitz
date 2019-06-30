#include "minimap.hpp"

void Minimap::update(float dt) {
    for (auto entity : needs_update) {
        auto position = entity->get_coordinate().to_pixel();
        auto offset_x = std::abs(bounds.x - position.x);
        auto offset_y = std::abs(bounds.y - position.y);
        auto scale_x = config::minimap_width / bounds.width;
        auto scale_y = config::minimap_height / bounds.height;
        auto minimap_x = minimap_origin.x + offset_x * scale_x;
        auto minimap_y = minimap_origin.y + offset_y * scale_y;
        Sprite & sprite = entity_sprites.at(entity);
        sprite.set_position(minimap_x, minimap_y);
    }
    needs_update.clear();
}

void Minimap::add_entity(Entity & entity) {
    entity_sprites[&entity] = spritef.create(renders, "minimap-indicator");
    entity_sprites[&entity].set_layer(config::ui_layer + 1);
    needs_update.push_back(&entity);
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

Minimap::Minimap(Engine & engine) 
    : spritef(engine.spritef), renders(engine.uirender),
      bounds(engine.tilem.get_pixel_bounds())
{
    engine.signals.update.add_callback([&](float dt){
        system_update(dt);
    });
    engine.entitym.signals.entity_spawned.add_observer(this, [&](Entity & entity){
        system_add_entity(entity);
    });
    engine.moves.signals.entity_move.add_observer(this, [&](Entity & entity, auto &&){
        needs_update.push_back(&entity);
    });

    // add existing entities
    engine.entitym.map([&](Entity & entity){ system_add_entity(entity); });

    sf::Image img; img.loadFromFile("../sprites/tmp.png");
    auto pixels = img.getPixelsPtr();
    engine.sfml.texture.update(pixels, config::minimap_width, config::minimap_height, 0, 0);

    worldsprite = engine.spritef.create(engine.uirender, "minimap-world");

    minimap_origin = Position{0, config::winh - config::minimap_height};
    worldsprite.set_position(minimap_origin);
}
