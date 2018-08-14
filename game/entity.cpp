#include "entity.hpp"
#include <cassert>

Entity::Entity() {
}

void Entity::update(gfx::time_t dt) {
    animation.update(dt);
}

void Entity::set_cell(Cell * c) {
    assert(c != nullptr);
    cell = c;
    auto pos = cell->get_position();
    //pos.x += TILEW/2;
    //pos.y += TILEH/2;
    animation.sprite.set_position(pos);
}
