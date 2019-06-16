#include "coordinate.hpp"
#include "util.hpp"

Position::Position() {
}

Position::Position(float _x, float _y) : x(_x), y(_y) {
}

Position::Position(int _x, int _y) : x(_x), y(_y) {
}

Position::Position(const sf::Vector2f & v) : Position(v.x, v.y) {
}

Position::Position(const sf::Vector2i & v) : Position(v.x, v.y) {
}

float Position::distance_to(const Position & other) const {
    const auto dx = x - other.x == 0 ? 0 : std::pow(x - other.x, 2);
    const auto dy = y - other.y == 0 ? 0 : std::pow(y - other.y, 2);
    return dx + dy == 0 ? 0 : std::sqrt(dx + dy);
}

Position Position::operator-(const Position & other) const {
    return {x - other.x, y - other.y};
}

Position Position::operator+(const Position & other) const {
    return {x + other.x, y + other.y};
}

Position Position::operator*(const float z) const {
    return {x * z, y * z};
}

Position Position::operator/(const float z) const {
    return {x / z, y / z};
}

void Position::operator+=(const Position & other) {
    x += other.x;
    y += other.y;
}

Position::operator sf::Vector2f() const {
    return {x, y};
}

Position::operator sf::Vector2i() const {
    return {static_cast<int>(x), static_cast<int>(y)};
}

bool Position::operator==(const Position & other) const {
    return x == other.x && y == other.y;
}

bool Position::operator!=(const Position & other) const {
    return !operator==(other);
}

void Position::serialize(std::ostream & out) const {
    util::write(x, out);
    util::write(y, out);
}

void Position::deserialize(std::istream & in) {
    util::read(x, in);
    util::read(y, in);
}

/////////////////////////////////////////////////////////////////////// COORDINATE

template<int W, int H>
Coordinate<W, H>::Coordinate() {
}

template<int W, int H>
Coordinate<W, H>::Coordinate(float _x, float _y) : x(_x), y(_y) {
}

template<int W, int H>
Coordinate<W, H>::Coordinate(int _x, int _y) : x(_x), y(_y) {
}

template<int W, int H>
Coordinate<W, H>::Coordinate(const sf::Vector2f & v) : Coordinate(v.x, v.y) {
}

template<int W, int H>
Coordinate<W, H>::Coordinate(const sf::Vector2i & v) : Coordinate(v.x, v.y) {
}

template<int W, int H>
Coordinate<W, H>::Coordinate(const Position & pos) : Coordinate(pos.to_map<W, H>()) {
}

// snap to grid
template<int W, int H>
Coordinate<W, H> Coordinate<W, H>::to_grid() const {
    return {
        std::floor(x),
        std::floor(y)
    };
}

template<int W, int H>
float Coordinate<W, H>::distance_to(const Coordinate & other) const {
    const auto dx = x - other.x == 0 ? 0 : std::pow(x - other.x, 2);
    const auto dy = y - other.y == 0 ? 0 : std::pow(y - other.y, 2);
    return dx + dy == 0 ? 0 : std::sqrt(dx + dy);
}

template<int W, int H>
Coordinate<W, H>::operator sf::Vector2f() const {
    return {x, y};
}

template<int W, int H>
Coordinate<W, H>::operator Position() const { return to_pixel(); }

template<int W, int H>
Coordinate<W, H>::operator sf::Vector2i() const {
    return {static_cast<int>(x), static_cast<int>(y)};
}

template<int W, int H>
bool Coordinate<W, H>::operator==(const Coordinate & other) const {
    return x == other.x && y == other.y;
}

template<int W, int H>
bool Coordinate<W, H>::operator!=(const Coordinate & other) const {
    return !operator==(other);
}

template<int W, int H>
void Coordinate<W, H>::serialize(std::ostream & out) const {
    util::write(x, out);
    util::write(y, out);
}

template<int W, int H>
void Coordinate<W, H>::deserialize(std::istream & in) {
    util::read(x, in);
    util::read(y, in);
}

template class Coordinate<config::cellw, config::cellh>;
template class Coordinate<config::tilew, config::tileh>;
