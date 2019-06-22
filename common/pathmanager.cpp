#include "pathmanager.hpp"
#include <iostream>

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
    Path path;
    auto start = entity.get_coordinate();
    path.push_back(start);
    /* test path
    for (int i = 0; i < 4; i++)
        path.push_back(path.back() + Coordinate{1, -1});
    for (int y = 0; y < config::rows_per_tile; y++)
        path.push_back(path.back() + Coordinate{0, 1});
    for (int x = 0; x < config::cols_per_tile; x++)
        path.push_back(path.back() + Coordinate{-1, 0});
    for (int y = 0; y < config::rows_per_tile; y++)
        path.push_back(path.back() + Coordinate{0, -1});
    for (int x = 0; x < config::cols_per_tile; x++)
        path.push_back(path.back() + Coordinate{1, 0});
    */
    entity_path_map[&entity] = path;
}

PathManager::PathManager(TileManager & tm, MovementSystem & ms) : tilem(tm), moves(ms) {
}
