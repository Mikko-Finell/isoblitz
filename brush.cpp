#include "brush.hpp"

Brush::Brush() {
    empty_tile = Tile::Type{Coordinate{0,0}, true};
    default_tile = Tile::Type{Coordinate{0,TILEH*2}, false};
    current_tile = default_tile;
}

void Brush::attach(UI & _ui) {
    ui = &_ui;
}

Coordinate Brush::coordinate() const {
    return tile_center_at(ui->mouse_pos());
}

void Brush::draw(std::vector<sf::Vertex> & vertices) const {
    Tile tile{coordinate()};
    tile.replace_with(current_tile);
    tile.draw(vertices);
    auto sz = vertices.size();
    for (int i = 0; i < 4; i++) {
	vertices[sz - i - 1].color.a /= 2;
    }
}
