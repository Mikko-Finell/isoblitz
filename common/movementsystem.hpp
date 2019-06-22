#ifndef movementsystem_hpp
#define movementsystem_hpp

#include "observer.hpp"
#include "coordinate.hpp"
#include "entity.hpp"
#include <unordered_map>
#include <list>

struct Movement {
    float speed = 100.0;
    float velocity = 0.0;
    sf::Vector2f unit_vector;
    Coordinate target;
};

class MovementSystem : public Observer {
    std::list<Entity *> remove_queue;
    std::unordered_map<Entity *, Movement> entity_move_map;

public:
    struct {
        Signal<Entity &, const sf::Vector2f &> entity_moved;
    } signals;

    Movement & get(Entity * entity);
    void update(float dt);
    void set_target(Entity & entity, const Coordinate & target);
};

#endif
