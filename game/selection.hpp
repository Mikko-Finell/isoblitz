#ifndef selection_hpp
#define selection_hpp

#include "common/entity.hpp"
#include "common/sprite.hpp"
#include "common/spritefactory.hpp"
#include <vector>

class SelectionManager {
    RenderSystem & render;
    Sprite sprite;
    std::vector<Sprite> sprites;
    std::vector<Entity *> selected_entities;
    sf::IntRect rect;

public:
    SelectionManager(RenderSystem & rs, SpriteFactory & spritef);
    void start(float x, float y);
    void start(const sf::Vector2f & v);
    void update(float x, float y);
    void update(const sf::Vector2f & v);
    void select_current_rect();
};

#endif
