#include "movementsystem.hpp"
#include <iostream>

Movement & MovementSystem::get(Entity * entity) {
    return entity_move_map.at(entity);
}

void MovementSystem::update(float dt) {
    for (auto entityptr : remove_queue) {
        entity_move_map.erase(entityptr);
        signals.entity_moved(*entityptr, sf::Vector2f{0, 0});
    }
    remove_queue.clear();
    for (auto & pair : entity_move_map) {
        Entity & entity = *pair.first;
        Movement & movement = pair.second;
        Coordinate coordinate = entity.get_coordinate();
        sf::Vector2f full_vector = movement.target - coordinate;
        sf::Vector2f move_vector = movement.unit_vector * movement.velocity * dt;

        auto magnitude = [](const sf::Vector2f & v){ return std::sqrt(v.x*v.x+v.y*v.y); };
        if (magnitude(full_vector) < magnitude(move_vector)) {
            move_vector = full_vector;
        }
        entity.set_coordinate(coordinate + move_vector);
    }
}

void MovementSystem::set_target(Entity & entity, const Coordinate & target) {
    if (entity_move_map.count(&entity) == 0) {
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
    signals.entity_moved(entity, movement.unit_vector);
}
