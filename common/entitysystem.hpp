#ifndef entitysystem_hpp
#define entitysystem_hpp

#include "entity.hpp"
#include "observer.hpp"
#include <unordered_set>

class EntitySystem : public Observer {
    std::unordered_set<Entity *> _remove_queue;
    std::unordered_set<Entity *> _entities;

    virtual void update(float dt);
    virtual void add_entity(Entity &);
    virtual void remove_entity(Entity &);
    virtual void clear();
   
public:
    virtual ~EntitySystem();

    void system_update(float dt = 0);
    void system_add_entity(Entity & entity);
    inline void system_add_entity(Entity * entity) {
        system_add_entity(*entity);
    }
    void system_remove_entity(Entity & entity);
    inline void system_remove_entity(Entity * entity) {
        system_remove_entity(*entity);
    }
    void queue_remove(Entity & entity);
    inline void queue_remove(Entity * entity) {
        queue_remove(*entity);
    }
    void system_clear();
};

#endif
