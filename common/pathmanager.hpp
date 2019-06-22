#ifndef pathmanager_hpp
#define pathmanager_hpp

#include "movementsystem.hpp"
#include "entity.hpp"
#include "tilemanager.hpp"
#include <list>
#include <unordered_map>

using Path = std::list<Coordinate>;

class PathManager : public Observer {
    std::list<Entity *> remove_queue;
    std::unordered_map<Entity *, Path> entity_path_map;
    TileManager & tilem;
    MovementSystem & moves;

public:
    PathManager(TileManager & tm, MovementSystem & ms);
    void update();
    void find_path(Entity & entity, const Coordinate & target);
};

#endif
