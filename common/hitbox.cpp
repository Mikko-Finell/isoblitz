#include "hitbox.hpp"

Hitbox::Hitbox() {
}

Hitbox::Hitbox(int offset_x, int offset_y, int w, int h) {
    offset = sf::Vector2i{offset_x, offset_y};
    screencoords = sf::FloatRect{0, 0, static_cast<float>(w), static_cast<float>(h)};
}

void Hitbox::set_position(const Position & pos) {
    screencoords.left = pos.x - offset.x;
    screencoords.top = pos.y - offset.y;
}
