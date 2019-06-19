#include "tilecursor.hpp"

TileCursor::~TileCursor() {
}

TileCursor::TileCursor(Engine & engine) : engine(engine) {
    Position center{config::winw/2, config::winh/2};
    Coordinate _cursor_coord{center};
    Coordinate cursor_coord = _cursor_coord.to_grid();
    cursor = engine.tilef.create_unmanaged(engine.wrender, 1);
    cursor.set_coordinate(cursor_coord);
}

void TileCursor::set_tile_type(Tile::ID id) {
    const auto coord = cursor.get_coordinate();
    //engine.tilem.destroy(cursor);
    cursor = engine.tilef.create_unmanaged(engine.wrender, id);
    cursor.set_coordinate(coord);
}

void TileCursor::update_mousepos(const Position & pos) {
    // note: All entities are given an offset, so that when we say 
    // sprite.set_position the actual x,y result is something that makes 
    // sense like the center of a tile or the feet of a unit. That is why 
    // this is required, it's like the mouse cursor's offset.
    const auto y = pos.y + config::cellh / 2;
    Position p{pos.x, y};
    auto coord = Coordinate(p).to_grid();
    cursor.set_coordinate(coord);
}

Tile::ID TileCursor::get_id() const {
    return cursor.get_id();
}

Coordinate TileCursor::get_coordinate() const {
    return cursor.get_coordinate();
}
