#ifndef selectionmanager_hpp
#define selectionmanager_hpp

#include "entitymanager.hpp"
#include "sprite.hpp"
#include "spritefactory.hpp"
#include "input.hpp"
#include "observer.hpp"
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <list>
#include <functional>

class SelectionManager : public Observer {
    SpriteFactory & spritef;
    EntityManager & entitym;
    Sprite selection_sprite;
    sf::FloatRect rect;

    std::vector<Entity *> remove_queue;
    std::unordered_map<Entity *, Sprite> entity_sprite_map;
    std::unordered_set<Entity *> entities;

    void cleanup();

public:
    input::Context ctx;

    SelectionManager(SpriteFactory & spritef, EntityManager & em);

    void start(const Position & position);
    void set_rect_position(const Position & position);
    void select_current_rect();
    void update();
    void map(const std::function<void(Entity &)> & fn);
};

#endif
