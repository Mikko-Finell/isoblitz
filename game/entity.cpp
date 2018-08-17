#include "entity.hpp"
#include <cassert>

void Entity::update(time_t dt) {
    animation.update(dt);
}

void Entity::set_cell(const cell_t & c) {
    cell = c;
    hitbox.set_position(cell);
    auto pos = util::to_pixel(cell);
    animation.sprite.set_position(pos.x, pos.y);
}
