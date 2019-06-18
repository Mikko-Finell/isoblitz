#ifndef entitymanager_hpp
#define entitymanager_hpp

#include "entity.hpp"
#include <list>

/**
 * EntityManager
 * Responsible for the lifetime of entities.
 */
class EntityFactory;
class EntityManager { // : public Observer {
    std::list<Entity> entities;

public:
    virtual ~EntityManager() {}
    EntityManager();

    Entity * alloc();
    void destroy(Entity * entity);
    std::vector<Entity *> get_all();

    // returns existing entity with uid or nullptr
    Entity * get(const uid_t & uid);

    void clear();
    void serialize(std::ostream & out) const;
    void deserialize(EntityFactory * ef, std::istream & in);
};

#endif
