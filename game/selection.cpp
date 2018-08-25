#include "selection.hpp"
#include <iostream>

SelectionManager::SelectionManager(RenderSystem & rs, SpriteFactory & sf)
    : render(rs), spritef(sf), sprite(rs)
{
    sprite = spritef.get("game-ui", "selection-rect");
    sprite.set_layer(4);
    sprite.hide();
}

void SelectionManager::start(float x, float y) {
    rect = sf::IntRect(x, y, 1, 1);
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
}

void SelectionManager::add_entity(Entity * entity) {
    entities.push_back(entity);
    sprites.emplace_back(spritef.get("test", "hitbox-bg"));
    auto & hsprite = sprites.back();
    render.add(hsprite);

    hsprite.set_layer(TILE_INDICATOR_LAYER);
    hsprite.set_screencoords(entity->hitbox);



    std::cout << "Selm::hsprite is " << hsprite.info() << std::endl;
}
