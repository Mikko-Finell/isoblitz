#include "selection.hpp"
#include <iostream>

SelectionManager::SelectionManager(RenderSystem & rs, SpriteFactory & spritef)
    : render(rs), sprite(rs)
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
