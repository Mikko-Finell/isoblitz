#ifndef entitymanager_hpp
#define entitymanager_hpp

#include "entityfactory.hpp"
#include <list>

/**
 * EntityManager
 * Responsible for the lifetime of entities.
 */
class EntityManager : public Observer {
    std::list<Entity *> entities;

public:
    virtual ~EntityManager() {
    }

    // returns existing entity with uid or nullptr
    Entity * get(const uid_t & uid);

    // add entity to the manager, uid must be unique
    void add_entity(Entity * entity);

    std::vector<Entity *> get_all();

    /*
    // if entity with id exists, deletes it completely
    void destroy(const uid_t & id);

    inline void destroy(Entity & entity) {
        destroy(entity.get_uid());
    }

    inline void destroy(Entity * entity) {
        if (entity) {
            destroy(*entity);
        }
    }
    */

    // destroy all entities
    void clear();

    void serialize(std::ostream & out) const;
    void deserialize(EntityFactory & ef, std::istream & in);
};

#endif
