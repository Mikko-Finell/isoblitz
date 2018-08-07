#include <iostream>
#include <utility>
#include "common/helper.hpp"
#include "brush.hpp"

Brush::Brush(Map & m, gfx::Manager & sm) : map(m), tile(sm), hltile(sm) {
    tile.set_sprite(sf::Vector2i(0, 128));
    hltile.set_sprite(sf::Vector2i(128, 128));
}

void Brush::on_paint() {
    map.create(tile);
}

void Brush::on_erase() {
    map.remove(tile.coordinate());
}

void Brush::on_update_mousepos(const sf::Vector2f & pos) {
    tile.set_coordinate(tile_center_at(pos));
    hltile.set_coordinate(tile.coordinate());
}

void Brush::on_setsprite(const sf::Vector2i & coord) {
    tile.set_sprite(sf::Vector2i(coord));
}

void Brush::on_setblocked(bool b) {
    tile.set_blocked(b);
    std::cout << "Brush set to " << (tile.is_blocked() ? "blocked" : "open")
        << std::endl;
}
