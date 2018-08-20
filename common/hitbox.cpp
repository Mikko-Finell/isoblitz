#include "hitbox.hpp"

Hitbox::Hitbox() {
}

Hitbox::Hitbox(int offset_x, int offset_y, int w, int h) {
    rect = sf::IntRect{offset_x, offset_y, w, h};
}

void Hitbox::set_position(cell_t) {
}
