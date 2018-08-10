#include "tile.hpp"
#include "common/helper.hpp"
#include <cassert>

Tile::Tile(gfx::SpriteManager & spritem) : MapObject(spritem) {
    const auto offset = SPRIH / 4;

    blocked_sprite = gfx::Sprite{&spritem};
    blocked_sprite.set_spritecoord(sf::Vector2i(128, 0));
    blocked_sprite.set_origin(sf::Vector2i{0, -offset});
    blocked_sprite.set_size(sf::Vector2i{SPRIW, SPRIH});
    blocked_sprite.set_layer(get_layer() + 1);
    blocked_sprite.set_visible(is_blocked());
}

void Tile::set_coordinate(const sf::Vector2f & c) {
    MapObject::set_coordinate(c);
    blocked_sprite.set_position(logic_to_pixel(c));
}

void Tile::set_blocked(bool b) {
    MapObject::set_blocked(b);
    blocked_sprite.set_visible(is_blocked());
}

void Tile::center_at(const sf::Vector2f & pos) {
    auto v = pos;
    v.x -= HALFW;
    auto w = to_grid(v);
    set_coordinate(pixel_to_logic(w));
}

void Tile::set_layer(int layer) {
    MapObject::set_layer(layer);
    blocked_sprite.set_layer(layer + 1);
}

bool Tile::operator==(const Tile & t) const {
    return coordinate() == t.coordinate() && get_layer() == t.get_layer()
        && is_blocked() == t.is_blocked() && get_sprite() == t.get_sprite();
}
