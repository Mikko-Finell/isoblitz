#include "tile.hpp"
#include "common/util.hpp"
#include <cassert>

Tile::Tile(gfx::SpriteManager & spritem) : MapObject(spritem) {
    blocked_sprite = gfx::Sprite{&spritem};
    blocked_sprite.set_spritecoord(sf::Vector2i(128, 0));

    blocked_sprite.set_offset(TILEW/2, TILEH/2 + CELLH/2);

    blocked_sprite.set_size(SPRIW, SPRIH);
    blocked_sprite.set_layer(get_layer() + 1);
    blocked_sprite.set_visible(is_blocked());
}

void Tile::set_coordinate(const sf::Vector2f & c) {
    MapObject::set_coordinate(c);
    blocked_sprite.set_position(util::to_pixel(c));
}

void Tile::set_blocked(bool b) {
    MapObject::set_blocked(b);
    blocked_sprite.set_visible(is_blocked());
}

void Tile::set_layer(int layer) {
    MapObject::set_layer(layer);
    blocked_sprite.set_layer(layer + 1);
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
        && is_blocked() == t.is_blocked() && get_sprite() == t.get_sprite();
}
