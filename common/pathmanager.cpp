#include "pathmanager.hpp"
#include "state.hpp"
#include <CASE/timer.hpp>
#include <iostream>
#include <queue>

Path astar(const Coordinate & start, const Coordinate & target) {
    auto neighbors = [](const Coordinate & coordinate){
        std::list<Coordinate> ns;
        for (int x : {-1, 0, 1}) {
            for (int y : {-1, 0, 1}) {
                if (x == 0 and y == 0) continue;
                ns.push_back(coordinate + Coordinate{x, y});
            }
        }
        return ns;
    };
    using Cell = std::pair<Coordinate, float>;
    auto cmp = [&](const Cell & a, const Cell & b){
        return a.second > b.second;
    };
    auto heuristic = [&](const Coordinate & a, const Coordinate & b){
        return a.distance_to(b);
    };
    auto cost = [&](const Coordinate & a, const Coordinate & b){
        return 1;
    };
    std::priority_queue<Cell, std::vector<Cell>, decltype(cmp)> frontier{cmp};
    std::unordered_map<Coordinate, Coordinate, Coordinate::Hash> came_from;
    std::unordered_map<Coordinate, float, Coordinate::Hash> cost_so_far;

    frontier.push(std::make_pair(start, 0));
    came_from[start] = start;
    cost_so_far[start] = 0;

    {
    CASE::ScopeTimer timer{"Pathfinding: A*"};
    while (frontier.empty() == false) {
        auto current = frontier.top();
        frontier.pop();
        if (current.first == target) {
            break;
        }
        for (auto next : neighbors(current.first)) {
            auto new_cost = cost_so_far[current.first] + cost(current.first, next);
            if (cost_so_far.count(next) == 0 or new_cost < cost_so_far[next]) {
                cost_so_far[next] = new_cost;
                auto priority = new_cost + heuristic(target, next);
                frontier.push(std::make_pair(next, priority));
                came_from[next] = current.first;
            }
        }
    }
    }

    Path path;
    path.push_front(target);
    auto next = came_from[target];
    while (next != start) {
        path.push_front(next);
        next = came_from[next];
    }
    path.push_front(start);
    return path;
}

Path bestfirst(const Coordinate & start, const Coordinate & target) {
    auto neighbors = [](const Coordinate & coordinate){
        std::list<Coordinate> ns;
        for (int x : {-1, 0, 1}) {
            for (int y : {-1, 0, 1}) {
                if (x == 0 and y == 0) continue;
                ns.push_back(coordinate + Coordinate{x, y});
            }
        }
        return ns;
    };
    auto heuristic = [&](const Coordinate & coordinate){
        return coordinate.distance_to(target);
    };
    auto cmp = [&](const Coordinate & a, const Coordinate & b){
        return heuristic(a) > heuristic(b);
    };
    auto cost = [&](const Coordinate & a, const Coordinate & b){
        return 1;
    };
    std::priority_queue<Coordinate, std::vector<Coordinate>, decltype(cmp)> frontier{cmp};
    std::unordered_map<Coordinate, Coordinate, Coordinate::Hash> came_from;
    std::unordered_map<Coordinate, float, Coordinate::Hash> cost_so_far;

    frontier.push(start);
    came_from[start] = start;

    {
    CASE::ScopeTimer timer{"Pathfinding: Greedy Best First"};
    while (frontier.empty() == false) {
        auto current = frontier.top();
        frontier.pop();

        if (current == target) {
            break;
        }
        for (auto next : neighbors(current)) {
            if (came_from.count(next) == 0) {
                frontier.push(next);
                came_from[next] = current;
            }
        }
    }
    }
    Path path;
    path.push_front(target);
    auto next = came_from[target];
    while (next != start) {
        path.push_front(next);
        next = came_from[next];
    }
    path.push_front(start);
    return path;
}

std::list<Sprite> path_sprites;

PathManager::~PathManager() {
    path_sprites.clear();
}

PathManager::PathManager(TileManager & tm, MovementSystem & ms) : tilem(tm), moves(ms) {
}

void create_path_sprites(Path & path) {
    path_sprites.clear();
    auto & engine = StateManager::get_state("test");

    for (auto & coord: path) {
        auto & sprite = path_sprites.emplace_back(engine.spritef.create("debug1", "pathcell"));
        sprite.set_offset(8, 8);
        sprite.set_position(coord.to_pixel());
        sprite.set_layer(config::tile_indicator_layer+1);
    }
}

void PathManager::update() {
    for (Entity * entity : remove_queue) {
        entity_path_map.erase(entity);
    }
    remove_queue.clear();
    for (auto & pair : entity_path_map) {
        auto & entity = *pair.first;
        auto & path = pair.second;

        Coordinate current = entity.get_coordinate();
        Coordinate target = path.front();
        if (current == target) {
            path.pop_front();
            if (path.empty()) {
                remove_queue.push_back(pair.first);
            }
            else {
                target = path.front();
            }
            moves.set_target(entity, target);
        }
    }
}

void PathManager::find_path(Entity & entity, const Coordinate & target) {
    Coordinate start;
    if (entity_path_map.count(&entity) == 0) {
        start = entity.get_coordinate().to_grid();
    }
    else {
        start = entity_path_map[&entity].front();
        entity_path_map.clear();
    }

    if (start.distance_to(target) < 64)
        entity_path_map[&entity] = bestfirst(start, target);
    else
        entity_path_map[&entity] = astar(start, target);

    // debug show path
    create_path_sprites(entity_path_map[&entity]);
}

