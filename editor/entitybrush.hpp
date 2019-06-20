#ifndef entitybrush_hpp
#define entitybrush_hpp

#include "common/tile.hpp"
#include "common/engine.hpp"
#include <unordered_map>

class EntityBrush {
    EntityManager & entitym;
    std::unordered_map<Coordinate, Entity *, Coordinate::Hash> lookup;

public:
    EntityBrush(EntityManager & em);
    void add_entity(Entity::Type type, const Coordinate & coord);
    void remove_entity(const Entity::Type & type);
};

#endif
