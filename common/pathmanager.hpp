#ifndef pathmanager_hpp
#define pathmanager_hpp

#include "movementsystem.hpp"
#include "entitysystem.hpp"
#include "graph.hpp"
#include <list>
#include <unordered_map>

using Path = std::list<Coordinate>;

class PathManager : public EntitySystem {
    std::unordered_map<Entity *, Path> entity_path_map;
    Graph graph;
    MovementSystem & moves;

    void _update(float dt = 0) override;
    void _remove_entity(Entity & entity) override;

public:
    virtual ~PathManager();
    PathManager(MovementSystem & ms);
    void init(const Graph & graph);
    void find_path(Entity & entity, const Coordinate & target);
};

#endif
