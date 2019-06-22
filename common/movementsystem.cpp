#include "movementsystem.hpp"
#include <iostream>

MovementSystem::Movement & MovementSystem::get(Entity * entity) {
    return entity_move_map.at(entity);
}

void MovementSystem::update(float dt) {
    for (auto entityptr : remove_queue) {
        std::cout << "Removed " << entityptr << " from movementsystem.\n";
        entity_move_map.erase(entityptr);
    }
    remove_queue.clear();
    for (auto & pair : entity_move_map) {
        Entity & entity = *pair.first;
        Movement & movement = pair.second;
        Coordinate coordinate = entity.get_coordinate();
        Coordinate moved = coordinate + movement.unit_vector * movement.velocity * dt; 

        if (coordinate.distance_to(movement.target) < coordinate.distance_to(moved)) {
            entity.set_coordinate(movement.target);
        }   
        else {
            entity.set_coordinate(moved);
        }
    }
}

void MovementSystem::set_target(Entity & entity, const Coordinate & target) {
    if (entity_move_map.count(&entity) == 0) {
        std::cout << "Added " << &entity << " to movementsystem.\n";
        entity.signals.im_dead.add_observer(this, [&](Entity & e){
            remove_queue.push_back(&e);
        });
    }
    Coordinate current = entity.get_coordinate();
    Movement & movement = entity_move_map[&entity];
    const float distance = current.distance_to(target);
    if (distance == 0) {
        movement.unit_vector = sf::Vector2f{0, 0};
        remove_queue.push_back(&entity);
    }
    else {
        movement.velocity = distance / (10000.0 / movement.speed);
        movement.unit_vector = (target - current) / distance;
    }
    movement.target = target;
}
