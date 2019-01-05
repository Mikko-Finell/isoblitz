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

    // TODO: this isn't used--entitym handles it--good or bad?
    RenderSystem & render; 

    mutable uid_t next_id = 0;
    std::unordered_map<type_id_t, Entity> entities;

public:
    EntityFactory(AnimationFactory & af, RenderSystem & rs);
    
    // create an entity of type.
    // NOTE: the entity is not added to entitysystem.
    // TODO: consider whether entity should be added to entitysystem
    Entity get(const type_id_t & type) const;

    // get a list of all available types of entities
    std::vector<type_id_t> get_all_types() const;
};

#endif
