#include <utility>
#include "brush.hpp"

Brush::Brush() {
    tile = Tile::default_tile();
}

Coordinate Brush::coordinate() const {
    return tile.coordinate();
}

void Brush::draw(std::vector<sf::Vertex> & vertices) const {
    tile.draw(vertices);
    auto sz = vertices.size();
    for (int i = 0; i < 4; i++) {
	vertices[sz - i - 1].color.a *= 0.5;
    }
}

void Brush::recvevent(const Event & event) {
    if (event == Event::Paint) {
        Event create{Event::CreateTile};
        create.param = tile;
	emit(create);
    }
    else if (event == Event::Erase) {
	Event erase{Event::RemoveTile};
        erase.param = tile.coordinate();
	emit(erase);
    }
    else if (event == Event::MousePosition) {
        auto pos = std::get<Position>(event.param);
	tile.set_coordinate(tile_center_at(pos));
    }
    else if (event == Event::SetSprite) {
        auto coord = std::get<Coordinate>(event.param);
        tile.set_sprite(coord);
    }
}
