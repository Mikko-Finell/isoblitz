#ifndef pathmanager_hpp
#define pathmanager_hpp

#include "movementsystem.hpp"
#include "entity.hpp"
#include "graph.hpp"
#include <list>
#include <unordered_map>

using Path = std::list<Coordinate>;

class PathManager : public Observer {
    std::list<Entity *> remove_queue;
    std::unordered_map<Entity *, Path> entity_path_map;
    Graph graph;
    MovementSystem & moves;

public:
    ~PathManager();
    PathManager(MovementSystem & ms);
    void init(const Graph & graph);
    void update();
    void find_path(Entity & entity, const Coordinate & target);
};

#endif
