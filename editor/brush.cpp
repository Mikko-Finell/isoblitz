#include <iostream>
#include <utility>
#include "common/helper.hpp"
#include "brush.hpp"

Brush::Brush(Map & m) : map(m) {
    tile = Tile::default_tile();
}

Coordinate Brush::coordinate() const {
    return coordinate();
}

void Brush::draw(std::vector<sf::Vertex> & vertices) const {
    tile.draw(vertices);
    auto sz = vertices.size();
    for (int i = 0; i < 4; i++) {
	vertices[sz - i - 1].color.a *= 0.5;
    }
    auto hl = Tile::hl_tile(tile.coordinate());
    hl.draw(vertices);
}

void Brush::on_paint() {
    map.create(tile);
}

void Brush::on_erase() {
    map.remove(tile.coordinate());
}

void Brush::on_update_mousepos(const Position & pos) {
    tile.set_coordinate(tile_center_at(pos));
}

void Brush::on_setsprite(const Coordinate & coord) {
    tile.set_sprite(coord);
}

void Brush::on_setblocked(bool b) {
    tile.set_blocked(b);
    std::cout << "Brush set to " << (tile.is_blocked() ? "blocked" : "open")
        << std::endl;
}
