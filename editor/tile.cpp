#include "tile.hpp"
#include "common/util.hpp"
#include <cassert>
#include <iostream>

Tile::Tile(RenderSystem & rs) : MapObject(rs) {
    blocked_sprite.init(rs)
        .set_spritecoords({128, 0, 128, 128})
        .set_offset(TILEW/2, TILEH/2 + CELLH/2)
        .set_size(SPRIW, SPRIH)
        .set_layer(TILE_INDICATOR_LAYER);
}

void Tile::set_coordinate(const sf::Vector2f & c) {
    MapObject::set_coordinate(c);
    auto pos = util::to_pixel(c);
    blocked_sprite.set_position(pos.x, pos.y);
}

void Tile::set_blocked(bool b) {
    MapObject::set_blocked(b);
    if (is_blocked()) {
        blocked_sprite.hide();
    }
    else {
        blocked_sprite.show();
    }
}

bool Tile::intersects(const Tile & other) const {
    const auto mycoord = coordinate();
    sf::FloatRect myrect{mycoord.x, mycoord.y, COLS_PER_TILE, ROWS_PER_TILE};
    const auto othercoord = other.coordinate();
    sf::FloatRect otherrect{othercoord.x, othercoord.y, COLS_PER_TILE, ROWS_PER_TILE};
    return myrect.intersects(otherrect);
}

bool Tile::operator==(const Tile & t) const {
    return coordinate() == t.coordinate() && get_layer() == t.get_layer()
        && is_blocked() == t.is_blocked() && sprite == t.sprite;
}
