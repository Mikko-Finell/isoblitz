#ifndef entityfactory_hpp
#define entityfactory_hpp

#include "entity.hpp"
#include "spritefactory.hpp"
#include "animationfactory.hpp"
#include "rendersystem.hpp"
#include "util.hpp"
#include <unordered_map>

/**
 * EntityFactory
 * Holds all available entities.
 */
class EntityFactory {
    AnimationFactory & animf;
    std::unordered_map<Entity::Type, Entity> entities;

public:
    EntityFactory(AnimationFactory & af);
    
    // create an entity of type.
    Entity create(RenderSystem & rs, const Entity::Type & type) const;

    // get a list of all available types of entities
    std::vector<Entity::Type> get_all() const;
};

#endif
