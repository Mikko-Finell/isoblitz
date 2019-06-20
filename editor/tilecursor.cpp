#include "tilecursor.hpp"

TileCursor::~TileCursor() {
}

TileCursor::TileCursor(Engine & engine) : engine(engine) {
    Position center{config::winw/2, config::winh/2};
    Coordinate _cursor_coord{center};
    Coordinate cursor_coord = _cursor_coord.to_grid();
    cursor = engine.tilef.create(1);
    cursor.set_coordinate(cursor_coord);
}

void TileCursor::set_tile_type(Tile::ID id) {
    const auto coord = cursor.get_coordinate();
    cursor = engine.tilef.create(id);
    cursor.set_coordinate(coord);
}

void TileCursor::set_coordinate(const Coordinate & coord) {
    cursor.set_coordinate(coord.to_grid());
}

Tile::ID TileCursor::get_id() const {
    return cursor.get_id();
}

Coordinate TileCursor::get_coordinate() const {
    return cursor.get_coordinate();
}
