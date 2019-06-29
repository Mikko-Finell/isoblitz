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

std::string Hitbox::info() const {
    std::stringstream ss; ss << "Hitbox{"
        << " Offset(" << offset.x << "," << offset.y << ") ; Screencoords("
        << util::rect_to_str(screencoords) << ") }";
    return ss.str();
}
