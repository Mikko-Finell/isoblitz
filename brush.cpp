#include <utility>
#include "brush.hpp"

Brush::Brush() {
    empty_tile = Tile::Type{Coordinate{0,0}, true};
    default_tile = Tile::Type{Coordinate{0,TILEH*2}, false};
    current_tile = default_tile;
}

Coordinate Brush::coordinate() const {
    return current_coord;
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

void Brush::recvevent(Event event) {
    if (event.type == Event::Paint) {
	auto data = std::make_pair(current_tile, coordinate());
	Event event{Event::CreateTile, &data};
	emit(event);
    }
    else if (event.type == Event::Erase) {
	auto coord = coordinate();
	Event event{Event::RemoveTile, &coord};
	emit(event);
    }
    else if (event.type == Event::MousePosition) {
	auto pos = *static_cast<Position*>(event.data);
	current_coord = tile_center_at(pos);
    }
}
