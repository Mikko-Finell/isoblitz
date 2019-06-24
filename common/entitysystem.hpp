#ifndef entitysystem_hpp
#define entitysystem_hpp

#include "entity.hpp"
#include "observer.hpp"
#include <unordered_set>

class EntitySystem : public Observer {
    std::unordered_set<Entity *> _remove_queue;
    std::unordered_set<Entity *> _entities;

    virtual void _update(float dt);
    virtual void _update_single(Entity & entity);
    virtual void _add_entity(Entity &);
    virtual void _remove_entity(Entity &);
    virtual void _clear();
   
public:
    virtual ~EntitySystem();

    void update(float dt = 0);
    void add_entity(Entity & entity);
    inline void add_entity(Entity * entity) {
        add_entity(*entity);
    }
    void remove_entity(Entity & entity);
    inline void remove_entity(Entity * entity) {
        remove_entity(*entity);
    }
    void queue_remove(Entity & entity);
    inline void queue_remove(Entity * entity) {
        queue_remove(*entity);
    }
    void clear();
};

#endif
