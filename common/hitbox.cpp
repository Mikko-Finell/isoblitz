#include "hitbox.hpp"

Hitbox::Hitbox() {
}

Hitbox::Hitbox(int offset_x, int offset_y, int w, int h) {
    rect = sf::IntRect{offset_x, offset_y, w, h};
}

void Hitbox::set_position(cell_t) {
}

void Hitbox::serialize(std::ostream & out) const {
    util::write(rect.left, out);
    util::write(rect.top, out);
    util::write(rect.width, out);
    util::write(rect.height, out);
}

void Hitbox::deserialize(std::istream & in) {
    util::read(rect.left, in);
    util::read(rect.top, in);
    util::read(rect.width, in);
    util::read(rect.height, in);
}
