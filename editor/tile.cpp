#include <cassert>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "common/helper.hpp"
#include "tile.hpp"

void Tile::serialize(std::ostream & out) const {
    const sf::Vector2i c(coord);
    const int layer = get_layer();
    const bool blocked = is_blocked();

    write(c.x, out);
    write(c.y, out);
    write(layer, out);
    write(blocked, out);

    out << main_sprite;
}

void Tile::deserialize(std::istream & in) {
    sf::Vector2i c;
    int layer;
    bool blocked;

    read(c.x, in);
    read(c.y, in);
    read(layer, in);
    read(blocked, in);

    in >> main_sprite;
    
    set_coordinate(sf::Vector2i(c.x, c.y));
    set_layer(layer);
    set_blocked(blocked);
}

Tile::Tile(gfx::Manager & spritem) {
    const auto offset = SPRIH / 4;

    main_sprite = gfx::Sprite{&spritem};
    main_sprite.set_origin(sf::Vector2i{0, -offset});
    main_sprite.set_size(sf::Vector2i{SPRIW, SPRIH});

    blocked_sprite = gfx::Sprite{&spritem};
    blocked_sprite.set_spritecoord(sf::Vector2i(128, 0));
    blocked_sprite.set_origin(sf::Vector2i{0, -offset});
    blocked_sprite.set_size(sf::Vector2i{SPRIW, SPRIH});

    set_layer(z);
    set_blocked(false);
}
            
sf::Vector2i Tile::coordinate() const {
    return coord;
}

void Tile::set_coordinate(const sf::Vector2i & c) {
    coord = c;
    main_sprite.set_position(logic_to_pixel(c));
    blocked_sprite.set_position(logic_to_pixel(c));
}

void Tile::set_sprite(const sf::Vector2i & c) {
    main_sprite.set_spritecoord(c);
}

void Tile::set_blocked(bool b) {
    blocked = b;
    blocked_sprite.set_visible(blocked);
}

void Tile::center_at(const sf::Vector2f & pos) {
    auto v = pos;
    v.x -= HALFW;
    auto w = snap_to_grid(v);
    set_coordinate(pixel_to_logic(w));
}

void Tile::move(const sf::Vector2i & offset) {
    set_coordinate(coord + offset);
}

void Tile::set_layer(int layer) {
    z = layer;
    main_sprite.set_layer(z);
    blocked_sprite.set_layer(z + 1);
}

int Tile::get_layer() const {
    return z;
}

bool Tile::operator==(const Tile & t) const {
    return coord == t.coord && z == t.z && blocked == t.blocked
        && main_sprite == t.main_sprite;
}

bool Tile::is_blocked() const {
    return blocked;
}

std::string Tile::debug() const {
    std::stringstream ss;
    if (!blocked) ss << "Open ";
    else         ss << "Closed ";
    ss << "Tile at vec2i{" << coord.x << ", " << coord.y << "}";
    ss << " with sprite sf::Vector2i{" << spritecoord.x << ", "
        << spritecoord.y << "}";
    return ss.str();
}
