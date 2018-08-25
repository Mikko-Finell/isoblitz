#ifndef entitymanager_hpp
#define entitymanager_hpp

#include "entityfactory.hpp"
#include <list>

class EntityManager {
    EntityFactory & entityf;
    EntitySystem & entitys;
    RenderSystem & render;
    AnimationSystem & anims;
    std::list<Entity> entities;

public:
    EntityManager(EntityFactory & ef, EntitySystem & es,
                  RenderSystem & rs, AnimationSystem & as);
    Entity * create(const type_id_t & type);
    std::vector<Entity *> create(const type_id_t & type, std::size_t n);
    Entity * get(const uid_t & uid);
    std::vector<Entity *> get_all();
    void destroy(const uid_t & id);
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
