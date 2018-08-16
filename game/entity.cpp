#include "entity.hpp"
#include <cassert>

Entity::Entity() {
}

void Entity::update(time_t dt) {
    animation.update(dt);
    
    auto pos = util::to_pixel(cell);
    animation.sprite.set_position(pos.x, pos.y);
}

void Entity::set_cell(const cell_t & c) {
    cell = c;
}
