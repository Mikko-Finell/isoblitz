#ifndef entitymanager_hpp
#define entitymanager_hpp

#include "entityfactory.hpp"
#include <list>

/**
 * EntityManager
 * Manages the lifetime of entities.
 *
 * TODO: Consider if creating an entity should add it to
 * entitysystem, if not we don't need the reference.
 * TODO: 
 */
class EntityManager {
    EntityFactory & entityf;

    // TODO: this isn't used
    EntitySystem & entitys;

    RenderSystem & render;

    // TODO: this isn't used
    AnimationSystem & anims;

    std::list<Entity> entities;

public:
    EntityManager(EntityFactory & ef, EntitySystem & es,
                  RenderSystem & rs, AnimationSystem & as);
    Entity * create(const type_id_t & type);

    // create n entities of type
    std::vector<Entity *> create(const type_id_t & type, std::size_t n);

    // returns existing entity with uid or nullptr
    Entity * get(const uid_t & uid);

    std::vector<Entity *> get_all();

    // if entity with id exists, deletes it completely
    void destroy(const uid_t & id);

    // destroy all entities
    void clear();

    inline void destroy(Entity & entity) {
        destroy(entity.get_uid());
    }

    inline void destroy(Entity * entity) {
        if (entity) {
            destroy(*entity);
        }
    }

    void serialize(std::ostream & out) const;
    void deserialize(std::istream & in);
};

#endif
