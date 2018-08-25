#ifndef selection_hpp
#define selection_hpp

#include "common/entity.hpp"
#include "common/sprite.hpp"
#include "common/spritefactory.hpp"
#include <vector>
#include <list>

class SelectionManager {
    RenderSystem & render;
    SpriteFactory & spritef;
    Sprite sprite;
    std::list<Sprite> sprites;
    std::vector<Entity *> selected_entities;
    std::vector<Entity *> entities;
    sf::IntRect rect;

public:
    SelectionManager(RenderSystem & rs, SpriteFactory & spritef);
    void start(float x, float y);
    void start(const sf::Vector2f & v);
    void update(float x, float y);
    void update(const sf::Vector2f & v);
    void select_current_rect();

    void add_entity(Entity * entity);
};

#endif
