#ifndef coordinate_hpp
#define coordinate_hpp

#include <SFML/System/Vector2.hpp>
#include <string>
#include <cmath>

/**
 * Position
 * Represents a visual position in the gameworld.
 */
class Position {
public:
    float x = 0;
    float y = 0;

    Position() {
    }

    Position(float _x, float _y) : x(_x), y(_y) {
    }

    Position(int _x, int _y) : x(_x), y(_y) {
    }
    
    Position(const sf::Vector2f & v) : Position(v.x, v.y) {
    }

    Position(const sf::Vector2i & v) : Position(v.x, v.y) {
    }

    // convert pixel position to logical coordinates
    // using a grid with cells of width W and height H
    // and assuming isometric relationship between 
    // visual/logical coordinates
    template<int W, int H>
    sf::Vector2f to_map() const {
        return {
            x / W + y / H,
            y / H - x / W
        };
    }

    float distance_to(const Position & other) const {
        const auto dx = x - other.x == 0 ? 0 : std::pow(x - other.x, 2);
        const auto dy = y - other.y == 0 ? 0 : std::pow(y - other.y, 2);
        return dx + dy == 0 ? 0 : std::sqrt(dx + dy);
    }

    Position operator-(const Position & other) const {
        return {x - other.x, y - other.y};
    }

    Position operator+(const Position & other) const {
        return {x + other.x, y + other.y};
    }

    Position operator*(const float z) const {
        return {x * z, y * z};
    }

    Position operator/(const float z) const {
        return {x / z, y / z};
    }

    void operator+=(const Position & other) {
        x += other.x;
        y += other.y;
    }

    operator sf::Vector2f() const {
        return {x, y};
    }

    operator sf::Vector2i() const {
        return {static_cast<int>(x), static_cast<int>(y)};
    }

    bool operator==(const Position & other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Position & other) const {
        return !operator==(other);
    }

    void serialize(std::ostream & out) const;
    void deserialize(std::istream & in);
    std::string info() const {
        return "Position(" + std::to_string(x) 
            + ", " + std::to_string(y) + ")";
    }
};

/**
 * Coordinate
 * Represents a coordinate in game logic.
 * W and H params signify the width and height of grid cells.
 * note: Why should Coordinate need to know W and H?
 */
template<int W, int H>
class Coordinate {
public:
    float x = 0;
    float y = 0;

    Coordinate() {
    }

    Coordinate(float _x, float _y) : x(_x), y(_y) {
    }

    Coordinate(int _x, int _y) : x(_x), y(_y) {
    }
    
    Coordinate(const sf::Vector2f & v) : Coordinate(v.x, v.y) {
    }

    Coordinate(const sf::Vector2i & v) : Coordinate(v.x, v.y) {
    }

    Coordinate(const Position & pos) : Coordinate(pos.to_map<W, H>()) {
    }

    // snap to grid
    Coordinate to_grid() const {
        return {
            std::floor(x),
            std::floor(y)
        };
    }

    // convert to visual position, assuming
    // an isometric relationship between logical coordinates
    // and visual positions in the gameworld
    Position to_pixel() const { 
        return {
            (x - y) * W * 0.5f,
            (x + y) * H * 0.5f
        };
    }

    float distance_to(const Coordinate & other) const {
        const auto dx = x - other.x == 0 ? 0 : std::pow(x - other.x, 2);
        const auto dy = y - other.y == 0 ? 0 : std::pow(y - other.y, 2);
        return dx + dy == 0 ? 0 : std::sqrt(dx + dy);
    }

    operator sf::Vector2f() const {
        return {x, y};
    }

    // TODO easy
    // operator Position() const { return to_pixel(); }

    operator sf::Vector2i() const {
        return {static_cast<int>(x), static_cast<int>(y)};
    }

    bool operator==(const Coordinate & other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Coordinate & other) const {
        return !operator==(other);
    }

    void serialize(std::ostream & out) const;
    void deserialize(std::istream & in);
    std::string info() const {
        return "Coordinate(" + std::to_string(x) 
            + ", " + std::to_string(y) + ")";
    }
};

#endif
