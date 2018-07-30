#include <cassert>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "common/helper.hpp"
#include "tile.hpp"

Tile Tile::empty_tile(const Coordinate & c) {
    Tile tile{c};
    tile.set_sprite(Coordinate{0, 0});
    tile.set_blocked(true);
    return tile;
}

Tile Tile::default_tile(const Coordinate & c) {
    Tile tile{c};
    tile.set_sprite(Coordinate{0, 128});
    tile.set_blocked(false);
    return tile;
}

Tile Tile::blocked_tile(const Coordinate & c) {
    Tile tile{c};
    tile.set_sprite(Coordinate{128, 0});
    return tile;
}

Tile Tile::hl_tile(const Coordinate & c) {
    Tile tile{c};
    tile.set_sprite(Coordinate{128, 128});
    return tile;
}

Tile::Tile() {}
Tile::Tile(const Coordinate & c) : coord(c) {}

void Tile::serialize(std::ostream & out) const {
    out << coord;
    out << spritecoord;
    write(blocked, out);
}

void Tile::deserialize(std::istream & in) {
    in >> coord;
    in >> spritecoord;
    read(blocked, in);
}

Coordinate Tile::coordinate() const {
    return coord;
}

void Tile::set_coordinate(const Coordinate & c) {
    coord = c;
}

void Tile::set_sprite(const Coordinate & c) {
    spritecoord = c;
}

void Tile::set_blocked(bool b) {
    blocked = b;
}

Tile Tile::from_position(const Position & pos) {
    Tile tile;
    tile.center_at(pos);
    return tile;
}

void Tile::center_at(const Position & pos) {
    auto v = pos;
    v.x -= HALFW;
    auto w = snap_to_grid(v);
    coord = pixel_to_logic(w);
}

void Tile::move(const Coordinate & offset) {
    coord += offset;
}

Tile Tile::moved(const Coordinate & offset) const {
    Tile copy = *this;
    copy.move(offset);
    return copy;
}

bool Tile::operator==(const Tile & t) const {
    return coord == t.coord && spritecoord == t.spritecoord
           && blocked == t.blocked;
}

bool Tile::operator<(const Tile & t) const {
    if (t.coord.y == coord.y) {
	return t.coord.x > coord.x;
    }
    else {
	return t.coord.y > coord.y;
    }
}

bool Tile::is_empty_tile() const {
    return spritecoord == Coordinate{0, 0};
}

bool Tile::is_blocked() const {
    return blocked;
}

void Tile::draw(std::vector<sf::Vertex> & vertices) const {
    static float sprite_w = TILEW;
    static float sprite_h = TILEH * 2;

    auto pixel_pos = logic_to_pixel(coord);
    auto offset = sprite_h / 4;

    auto topleft = pixel_pos + sf::Vector2f{0, -offset};
    auto topright = pixel_pos + sf::Vector2f{TILEW, -offset};
    auto botright = pixel_pos + sf::Vector2f{TILEW, sprite_h - offset};
    auto botleft = pixel_pos + sf::Vector2f{0, sprite_h - offset};

    // sprite coords
    auto c_topleft = spritecoord + sf::Vector2f{0, 0};
    auto c_topright = spritecoord + sf::Vector2f{sprite_w, 0};
    auto c_botright = spritecoord + sf::Vector2f{sprite_w, sprite_h};
    auto c_botleft = spritecoord + sf::Vector2f{0, sprite_h};

    vertices.push_back(sf::Vertex{topleft, c_topleft});
    vertices.push_back(sf::Vertex{topright, c_topright});
    vertices.push_back(sf::Vertex{botright, c_botright});
    vertices.push_back(sf::Vertex{botleft, c_botleft});

    if (is_blocked()) {
        auto blocked_tile = Tile::blocked_tile(coordinate());
        blocked_tile.draw(vertices);
    }
}

std::string Tile::debug() const {
    std::stringstream ss;
    ss << "Tile at "<<coord.debug();
    ss << " with sprite " << spritecoord.debug();
    return ss.str();
}

Tile::~Tile() {}
