#include <iostream>
#include <utility>
#include "common/helper.hpp"
#include "brush.hpp"

Brush::Brush() {
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
    else if (event == Event::SetBlocked) {
        if (auto bptr = std::get_if<bool>(&event.param)) {
            std::cout << "a\n";
            tile.set_blocked(*bptr);
        }
        else {
            tile.set_blocked(!tile.is_blocked());
            std::cout << "b\n";
        }
        std::cout << "Brush set to " << (tile.is_blocked() ? "blocked" : "open")
            << std::endl;
    }
}
