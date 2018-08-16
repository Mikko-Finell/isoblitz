#ifndef __selection_hpp__
#define __selection_hpp__

#include "entity.hpp"
#include "common/sprite.hpp"
#include <vector>

class SelectionManager {
    SpriteManager & spritem;
    Sprite selection_rect;
    std::vector<Sprite> sprites;
    std::vector<Entity *> selected_entities;
    sf::IntRect rect;

public:
    SelectionManager(SpriteManager & sm);
    void start(float x, float y);
    void start(const sf::Vector2f & v);
    void update(float x, float y);
    void update(const sf::Vector2f & v);
    void select_current_rect();
};

#endif
