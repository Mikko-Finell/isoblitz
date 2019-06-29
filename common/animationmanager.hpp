#ifndef animationmanager_hpp
#define animationmanager_hpp

#include "types.hpp"
#include "entitysystem.hpp"
#include "stl.hpp"

/**
 * AnimationManager
 * Abstraction layer that handles the details of which
 * sequence and animation should play in response to various
 * actions the entity performs.
 */
class AnimationManager : public EntitySystem {
    std::unordered_map<Entity *, directions::Type> direction;
    std::unordered_map<Entity *, actions::Type> action;

    void update(Entity & entity);
    void add_entity(Entity & entity) override;
    void remove_entity(Entity & entity) override;

public:
    void on_entity_move(Entity & entity, const sf::Vector2f & vector);
    void on_entity_idle(Entity & entity);
    void on_entity_fight(Entity & entity);
    void on_entity_die(Entity & entity);
};

#endif
