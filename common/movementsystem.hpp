#ifndef movementsystem_hpp
#define movementsystem_hpp

#include "observer.hpp"
#include "coordinate.hpp"
#include "entity.hpp"
#include "entitysystem.hpp"
#include "stl.hpp"

struct Movement {
    float speed = 100.0;
    float velocity = 0.0;
    sf::Vector2f unit_vector;
    Coordinate target;
};

class MovementSystem : public EntitySystem {
    std::unordered_map<Entity *, Movement> entity_move_map;
    void update(float dt) override;
    void remove_entity(Entity & entity) override;

public:
    struct {
        Signal<Entity &, const sf::Vector2f &> entity_move;
    } signals;

    Movement & get(Entity * entity);
    void set_target(Entity & entity, const Coordinate & target);
};

#endif
