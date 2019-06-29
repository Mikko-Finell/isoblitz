#ifndef entitymanager_hpp
#define entitymanager_hpp

#include "entity.hpp"
#include "entityfactory.hpp"
#include "serialize.hpp"
#include "rendersystem.hpp"
#include "observer.hpp"
#include <list>
#include <unordered_map>
#include <functional>

class EntityManager {
    EntityFactory & factory;
    std::list<Entity> entities;
    std::list<Entity *> destroy_queue;

public:
    struct {
        Signal<Entity &> entity_died;
        Signal<Entity &> entity_spawned;
    } signals;

    EntityManager(EntityFactory & ef);
    void update(float dt);
    Entity & create(const Entity::Type & type);
    void destroy(Entity & entity);
    void queue_destroy(Entity & entity);
    void clear();
    void map(const std::function<void(Entity &)> & fn);

    Entity * get_at_coordinate(const Coordinate & coord);
    std::list<Entity *> get_in_region(const Coordinate::Region & region);
    std::list<Entity *> get_in_region(const Position::Region & region);

    void deserialize(IOReader & in);
    void serialize(IOWriter & in) const;
};

#endif
