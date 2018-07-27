#include <cassert>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "helper.hpp"
#include "tile.hpp"

Tile::Type::Type()
    : spritecoord(0, 0), blocked(true) 
{
}

Tile::Type::Type(const Coordinate coords, bool blocked) 
    : spritecoord(coords), blocked(blocked) 
{
}

bool Tile::Type::operator==(const Tile::Type & other) {
    return spritecoord == other.spritecoord;
}

bool Tile::Type::operator!=(const Tile::Type & other) {
    return !(*this == other);
}

Tile::Tile() {}
Tile::Tile(const Coordinate & c) : coord(c) {}

void Tile::serialize(std::ostream & out) {
    out << coord;
    assert(history.size() >= 1);
    auto tiletype = type();
    out << tiletype.spritecoord;
    write(tiletype.blocked, out);
}

void Tile::deserialize(std::istream & in) {
    in >> coord;
    Coordinate spritecoord;
    in >> spritecoord;
    bool blocked = true;
    read(blocked, in);
    auto t = replace_with(Tile::Type{spritecoord, blocked});
    assert(t == true);
}

bool Tile::replace_with(const Tile::Type & other) {
    if (history.empty() || history.back() != other) {
	history.push_back(other);
	return true;
    }
    return false;
}

Tile::Type Tile::type() const {
    return history.back();
}

Coordinate Tile::coordinate() const {
    return coord;
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

bool Tile::operator==(const Tile & t) const {
    return t.coord == coord;
}

bool Tile::operator<(const Tile & t) const {
    if (t.coord.y == coord.y) {
	return t.coord.x > coord.x;
    }
    else {
	return t.coord.y > coord.y;
    }
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
    sf::Vector2f spr_xy = history.back().spritecoord;
    auto c_topleft = spr_xy + sf::Vector2f{0, 0};
    auto c_topright = spr_xy + sf::Vector2f{sprite_w, 0};
    auto c_botright = spr_xy + sf::Vector2f{sprite_w, sprite_h};
    auto c_botleft = spr_xy + sf::Vector2f{0, sprite_h};

    vertices.push_back(sf::Vertex{topleft, c_topleft});
    vertices.push_back(sf::Vertex{topright, c_topright});
    vertices.push_back(sf::Vertex{botright, c_botright});
    vertices.push_back(sf::Vertex{botleft, c_botleft});
}

std::string Tile::debug() const {
    std::stringstream ss;
    ss << "Tile at "<<coord.x<<","<<coord.y<<" \n";
    return ss.str();
}

Tile::~Tile() {}
