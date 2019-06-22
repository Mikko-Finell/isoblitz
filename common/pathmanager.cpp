#include "pathmanager.hpp"
#include <iostream>
#include <queue>

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
    auto cmp = [target](const Coordinate & a, const Coordinate & b){
        return a.distance_to(target) > b.distance_to(target);
    };
    auto cost = [&cmp](const Coordinate & a, const Coordinate & b){
        return cmp(a, b);
    };
    auto heuristic = [](const Coordinate & a, const Coordinate & b){
        return std::abs(a.x - b.x) + std::abs(a.y - b.y);
    };
    std::priority_queue<Coordinate, std::vector<Coordinate>, decltype(cmp)> frontier{cmp};
    std::unordered_map<Coordinate, Coordinate, Coordinate::Hash> came_from;
    std::unordered_map<Coordinate, float, Coordinate::Hash> cost_so_far;

    //auto start = entity.get_coordinate().to_grid();
    Coordinate start;
    if (entity_path_map.count(&entity) == 0) {
        start = entity.get_coordinate();
    }
    else {
        start = entity_path_map[&entity].front();
        entity_path_map.clear();
    }
    frontier.push(start);
    came_from[start] = start;
    cost_so_far[start] = 0;

    while (frontier.empty() == false) {
        auto current = frontier.top();
        if (current == target) {
            break;
        }
        for (auto next : neighbors(current)) {
            auto new_cost = cost_so_far[current] + cost(current, next);
            if (cost_so_far.count(next) == 0 or new_cost < cost_so_far[next]) {
                cost_so_far[next] = new_cost;
                //auto priority = new_cost + heuristic(goal, next);
                frontier.push(next);
                came_from[next] = current;
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

    entity_path_map[&entity] = path;
}

PathManager::PathManager(TileManager & tm, MovementSystem & ms) : tilem(tm), moves(ms) {
}
