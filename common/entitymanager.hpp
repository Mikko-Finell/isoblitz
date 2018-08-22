#ifndef entitymanager_hpp
#define entitymanager_hpp

#include "entity.hpp"
#include "entityfactory.hpp"
#include <list>
#include <unordered_set>

class EntityManager {
    bool is_updating = false;
    std::list<Entity> entities;
    std::list<std::list<Entity>::const_iterator> delete_queue;
    EntityFactory & entityf;

public:
    EntityManager(EntityFactory & ef);
    Entity * create(const type_id_t & type);
    void remove(const uid_t & id);
    inline void remove(Entity * entity) {
        remove(entity->get_uid());
    }
    inline void remove(Entity & entity) {
        remove(&entity);
    }
    void update(time_t dt);
    inline void clear() {
        entities.clear();
    }

    void serialize(std::ostream & out) const;
    void deserialize(std::istream & in, AnimationFactory & animf);
};

#endif
