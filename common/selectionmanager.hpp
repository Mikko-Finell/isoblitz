#ifndef selectionmanager_hpp
#define selectionmanager_hpp

#include "entitymanager.hpp"
#include "sprite.hpp"
#include "spritefactory.hpp"
#include "input.hpp"
#include <unordered_set>
#include <vector>
#include <list>

class SelectionManager {
    SpriteFactory & spritef;
    EntityManager & entitym;
    Sprite sprite;
    std::list<Sprite> hitbox_sprites;
    sf::FloatRect rect;
    std::list<std::pair<Entity *, Sprite> > entities;

public:
    input::Context ctx;

    SelectionManager(SpriteFactory & spritef, EntityManager & em);

    void start(const Position & position);
    void set_rect_position(const Position & position);
    void select_current_rect();
    void update();
};

#endif
