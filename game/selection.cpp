#include "selection.hpp"
#include <iostream>

SelectionManager::SelectionManager(RenderSystem & rs, SpriteFactory & spritef)
    : render(rs), selection_rect(rs)
{
    selection_rect = spritef.get("game", "selection-rect");
    selection_rect.set_layer(4);
    selection_rect.hide();
}

void SelectionManager::start(float x, float y) {
    rect = sf::IntRect(x, y, 1, 1);
    selection_rect.set_screencoords(rect);
    selection_rect.show();
}

void SelectionManager::start(const sf::Vector2f & v) {
    start(v.x, v.y);
}

void SelectionManager::update(float x, float y) {
    rect.width = x - rect.left;
    rect.height = y - rect.top;
    selection_rect.set_screencoords(rect);
}

void SelectionManager::update(const sf::Vector2f & v) {
    update(v.x, v.y);
}

void SelectionManager::select_current_rect() {
    selection_rect.hide();
}
