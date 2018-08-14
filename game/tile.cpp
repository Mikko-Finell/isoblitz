#include "tile.hpp"
#include "map.hpp"
#include <iostream>
using namespace std;

Cell::Cell(Tile * t) : tile(t) {
    neighbors.fill(nullptr);
}

sf::Vector2f Cell::get_position() const {
    return util::to_pixel<CELLW, CELLH>(coord);
}

void Cell::set_tile(Tile * t) {
    tile = t;
}

sf::Vector2f Cell::get_coordinate() const {
    return coord;
}

void Cell::set_coordinate(const sf::Vector2f & c) {
    coord = c;
}

// Tile

Tile::Tile(gfx::SpriteManager & spritem) : MapObject{spritem} {
}

void Tile::set_coordinate(const sf::Vector2f & c) {
    cells.fill(this);
    for (auto y = 0; y < ROWS_PER_TILE; y++) {
        for (auto x = 0; x < COLS_PER_TILE; x++) {
            auto cell = get_cell_localcoord(x, y, "set_coordinate");
            cell->set_coordinate({c.x + float(x), c.y + float(y)});
        }
    }
    MapObject::set_coordinate(c);
}

Cell * Tile::get_cell_globalcoord(int x, int y, const std::string & why) {
    x = x % COLS_PER_TILE;
    y = y % ROWS_PER_TILE;
    return get_cell_localcoord(x, y, why);
}

Cell * Tile::get_cell_localcoord(int x, int y, const std::string & why) {
    assert(x >= 0 && x < COLS_PER_TILE);
    assert(y >= 0 && y < ROWS_PER_TILE);
    return &cells[y * COLS_PER_TILE + x];
}

bool Tile::contains(const sf::Vector2f & c) const {
    auto coord = coordinate();
    sf::FloatRect box{coord.x, coord.y, COLS_PER_TILE, ROWS_PER_TILE};
    return box.contains(c);
}
