#include "pathmanager.hpp"

void PathManager::update() {
    for (Entity * entity : remove_queue) {
        entity_path_map.erase(entity);
    }
    remove_queue.clear();
    for (auto & pair : entity_path_map) {
        auto & entity = *pair.first;
        auto & path = pair.second;
        if (path.empty()) {
            remove_queue.push_back(pair.first);
        }
        else {
            Coordinate current = entity.get_coordinate();
            Coordinate target = path.front();
            if (current == target) {
                path.pop_front();
                if (path.empty() == false) {
                    target = path.front();
                }
                moves.set_target(entity, target);
            }
        }
    }
}

void PathManager::find_path(Entity & entity, const Coordinate & target) {
    Path path;
    auto start = entity.get_coordinate();
    for (int i = 0; i < 10; i++) {
        path.push_back(start);
        start.x += 1;
        start.y += 1;
    }
    entity_path_map[&entity] = path;
}

PathManager::PathManager(TileManager & tm, MovementSystem & ms) : tilem(tm), moves(ms) {
}
