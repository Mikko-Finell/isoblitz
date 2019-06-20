#ifndef entitymanager_hpp
#define entitymanager_hpp

#include "entity.hpp"
#include "entityfactory.hpp"
#include "animationfactory.hpp"
#include "rendersystem.hpp"
#include <list>
#include <unordered_map>

class EntityManager {
    EntityFactory & factory;
    std::list<Entity> entities;

public:
    EntityManager(EntityFactory & ef);
    Entity & create(const Entity::Type & type);
    void destroy(Entity & entity);
    void clear();

    Entity * get_at_coordinate(const Coordinate & coord);

    void deserialize(IOReader & in);
    void serialize(IOWriter & in) const;
};

#endif
