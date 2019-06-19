#ifndef entitybrush_hpp
#define entitybrush_hpp

#include "common/tile.hpp"
#include "common/engine.hpp"
//#include <unordered_set>
#include <list>

class EntityBrush {
    Engine & engine;
    std::list<Entity> entities;

public:
    EntityBrush(Engine & eng);
    void add_entity(Entity::Type type, const Tile::Coord & coord);
    void remove_entity(const Entity::Type & type);
};

#endif
