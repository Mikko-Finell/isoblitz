#ifndef selection_hpp
#define selection_hpp

#include "common/entity.hpp"
#include "common/sprite.hpp"
#include "common/spritefactory.hpp"
#include <unordered_set>
#include <vector>
#include <list>

class SelectionManager {
    RenderSystem & render;
    SpriteFactory & spritef;
    Sprite sprite;
    std::list<Sprite> hitbox_sprites;
    std::list<Sprite> select_sprites;
    std::unordered_set<Entity *> entities;
    sf::FloatRect rect;

public:
    std::vector<Entity *> selected_entities;

    SelectionManager(RenderSystem & rs, SpriteFactory & spritef);
    void start(float x, float y);
    void start(const sf::Vector2f & v);
    void update(float x, float y);
    void update(const sf::Vector2f & v);
    void select_current_rect();

    void add_entity(Entity * entity);
};

#endif
