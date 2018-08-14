#include "entity.hpp"
#include <cassert>

Entity::Entity() {
}

void Entity::update(gfx::time_t dt) {
    animation.update(dt);
}

void Entity::set_cell(const cell_t & c) {
    cell = c;
    auto pos = cell;
    animation.sprite.set_position(pos);
}
