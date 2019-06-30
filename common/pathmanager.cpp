#include "pathmanager.hpp"
#include <CASE/timer.hpp>

using Priority = float;
using Cell = std::pair<Coordinate, Priority>;
template<class cmp_t>
using PriorityQueue = std::priority_queue<Cell, std::vector<Cell>, cmp_t>;
using CoordinateMap = std::unordered_map<Coordinate, Coordinate, Coordinate::Hash>;
using CostMap = std::unordered_map<Coordinate, Priority, Coordinate::Hash>;

std::pair<bool, CoordinateMap> 
astar(const Coordinate & start, const Coordinate & target, Graph & graph) {
    auto cmp = [&](const Cell & a, const Cell & b){
        return a.second > b.second;
    };
    auto heuristic = [&](const Coordinate & a, const Coordinate & b){
        return a.distance_to(b);
    };
    auto cost = [&](const Coordinate & a, const Coordinate & b){
        return heuristic(a, b);
    };
    PriorityQueue<decltype(cmp)> frontier{cmp};
    CoordinateMap came_from;
    CostMap cost_so_far;

    frontier.push(std::make_pair(start, 0));
    came_from[start] = start;
    cost_so_far[start] = 0;

    bool found_path = false;

    CASE::ScopeTimer timer{"Pathfinding A*"};
    while (frontier.empty() == false) {
        auto current = frontier.top();
        frontier.pop();
        if (current.first == target) {
            found_path = true;
            break;
        }
        for (const Coordinate & next : graph.neighbors(current.first)) {
            auto new_cost = cost_so_far[current.first] + cost(current.first, next);
            if (cost_so_far.count(next) == 0 or new_cost < cost_so_far[next]) {
                cost_so_far[next] = new_cost;
                Priority priority = new_cost + heuristic(target, next);
                frontier.push(std::make_pair(next, priority));
                came_from[next] = current.first;
            }
        }
    }
    return std::make_pair(found_path, came_from);
}

std::pair<bool, CoordinateMap>
bestfirst(const Coordinate & start, const Coordinate & target, Graph & graph) {
    auto heuristic = [&](const Coordinate & coordinate){
        return coordinate.distance_to(target);
    };
    auto cmp = [&](const Coordinate & a, const Coordinate & b){
        return heuristic(a) > heuristic(b);
    };
    auto cost = [&](const Coordinate & a, const Coordinate & b){
        return a.distance_to(b);
    };
    std::priority_queue<Coordinate, std::vector<Coordinate>, decltype(cmp)> frontier{cmp};
    CoordinateMap came_from;

    frontier.push(start);
    came_from[start] = start;

    bool found_path = false;

    CASE::ScopeTimer timer{"Pathfinding Greedy Best First"};
    while (frontier.empty() == false) {
        auto current = frontier.top();
        frontier.pop();

        if (current == target) {
            found_path = true;
            break;
        }
        for (auto & next : graph.neighbors(current)) {
            if (came_from.count(next) == 0) {
                frontier.push(next);
                came_from[next] = current;
            }
        }
    }
    return std::make_pair(found_path, came_from);
}

template<class Algorithm>
Path _find_path(const Coordinate & start, const Coordinate & target, Graph & graph, Algorithm fn) {
    bool found_path;
    std::unordered_map<Coordinate, Coordinate, Coordinate::Hash> came_from;
    std::tie(found_path, came_from) = fn(start, target, graph);
    Path path;
    if (found_path) {
        path.push_front(target);
        auto next = came_from[target];
        while (next != start) {
            path.push_front(next);
            next = came_from[next];
        }
        std::cout << "Found path, " << path.size() << " steps.\n";
    }
    else {
        std::cout << "No path possible.\n";
    }
    path.push_front(start);
    return path;
}

//std::list<Sprite> path_sprites;

PathManager::~PathManager() {
    //path_sprites.clear();
}

PathManager::PathManager(MovementSystem & ms) : moves(ms) {
}

void create_path_sprites(Path & path) {
    //path_sprites.clear();
    //auto & engine = StateManager::get_state("test");

    //for (auto & coord: path) {
        //auto & sprite = path_sprites.emplace_back(engine.spritef.create("pathcell"));
        //sprite.set_offset(8, 8);
        //sprite.set_position(coord.to_pixel());
        //sprite.set_layer(config::tile_indicator_layer+1);
    //}
}

void PathManager::remove_entity(Entity & entity) {
    entity_path_map.erase(&entity);
}

void PathManager::update(float dt) {
    for (auto & pair : entity_path_map) {
        auto & entity = *pair.first;
        auto & path = pair.second;

        Coordinate current = entity.get_coordinate();
        Coordinate target = path.front();
        if (current == target) {
            path.pop_front();
            if (path.empty()) {
                queue_remove(entity);
            }
            else {
                target = path.front();
            }
            moves.set_target(entity, target);
        }
    }
}

void PathManager::init(const Graph & _graph) {
    graph = _graph;
}

void PathManager::find_path(Entity & entity, const Coordinate & target) {
    system_add_entity(entity);

    Coordinate start;
    if (entity_path_map.count(&entity) == 0) {
        start = entity.get_coordinate().to_grid();
    }
    else {
        start = entity_path_map[&entity].front();
        entity_path_map[&entity].clear();
    }

    entity_path_map[&entity] = _find_path(start, target, graph, astar);
    //create_path_sprites(entity_path_map[&entity]);
}

