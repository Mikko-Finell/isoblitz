#include "movementsystem.hpp"
#include "stl.hpp"

Movement & MovementSystem::get(Entity * entity) {
    return entity_move_map.at(entity);
}

void MovementSystem::update(float dt) {
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

void MovementSystem::remove_entity(Entity & entity) {
    signals.entity_move(entity, sf::Vector2f{0, 0});
    entity_move_map.erase(&entity);
}

void MovementSystem::set_target(Entity & entity, const Coordinate & target) {
    system_add_entity(entity);

    Coordinate current = entity.get_coordinate();
    Movement & movement = entity_move_map[&entity];
    movement.speed = 150.0;
    const float distance = current.distance_to(target);
    movement.unit_vector = sf::Vector2f{0, 0};
    movement.velocity = distance / (10000.0 / movement.speed);
    if (distance > 0) {
        movement.unit_vector = (target - current) / distance;
    }
    movement.target = target;
    signals.entity_move(entity, movement.unit_vector);
}
