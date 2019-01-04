#include "selection.hpp"
#include "common/util.hpp"
#include <iostream>

SelectionManager::SelectionManager(RenderSystem & rs, SpriteFactory & sf)
    : render(rs), spritef(sf), sprite(rs)
{
    sprite = spritef.get("game-ui", "selection-rect");
    sprite.set_layer(UI_LAYER);
    sprite.hide();
}

void SelectionManager::start(float x, float y) {
    rect = sf::FloatRect(x, y, 1.0f, 1.0f);
    sprite.set_screencoords(rect);
    sprite.show();
}

void SelectionManager::start(const sf::Vector2f & v) {
    start(v.x, v.y);
}

void SelectionManager::update(float x, float y) {
    rect.width = x - rect.left;
    rect.height = y - rect.top;
    sprite.set_screencoords(rect);
}

void SelectionManager::update(const sf::Vector2f & v) {
    update(v.x, v.y);
}

void SelectionManager::select_current_rect() {
    sprite.hide();
    selected_entities.clear();
    for (auto entity : entities) {
        if (rect.intersects(entity->hitbox)) {
            selected_entities.push_back(entity);
        }
    }

    select_sprites.clear();
    for (auto entity : selected_entities) {
        auto & sprite = select_sprites.emplace_back(
                spritef.get(entity->get_type(), "selection"));
        sprite.set_position(entity->cell.to_pixel());
        render.add(sprite);

        entity->signal.position.remove_callback("select-follow");
        auto follow = [&sprite](const Position & pos){
            sprite.set_position(pos);
        };
        entity->signal.position.add_callback("select-follow", follow);
    }
}

void SelectionManager::add_entity(Entity * entity) {
    entities.insert(entity);
    auto & hsprite = hitbox_sprites.emplace_back(spritef.get("test", "hitbox-bg"));
    render.add(hsprite);

    hsprite.set_layer(TILE_INDICATOR_LAYER);
    hsprite.set_screencoords(entity->hitbox);

    /*
    entity->signal.position.add_callback("hitbox-follow",
        [entity, &hsprite](const Position & pos){
            hsprite.set_screencoords(entity->hitbox);
        });
        */
}
