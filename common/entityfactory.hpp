#ifndef entityfactory_hpp
#define entityfactory_hpp

#include "entity.hpp"
#include "spritefactory.hpp"
#include "animationfactory.hpp"
#include "rendersystem.hpp"

/**
 * EntityFactory
 * Holds all available entities.
 */
class EntityFactory {
    AnimationFactory & animf;
    RenderSystem & default_rs;
    std::unordered_map<Entity::Type, Entity> entities;

public:
    EntityFactory(AnimationFactory & af, RenderSystem & rs);
    
    // create an entity of type.
    Entity create(RenderSystem & rs, const Entity::Type & type) const;
    Entity create(const Entity::Type & type) const;

    // get a list of all available types of entities
    std::vector<Entity::Type> get_all() const;
};

#endif
