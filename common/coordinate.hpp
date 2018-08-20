#ifndef coordinate_hpp
#define coordinate_hpp

#include <SFML/System/Vector2.hpp>
#include <string>
#include <cmath>

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

    template<int W, int H>
    sf::Vector2f to_map() const {
        return {
            x / W + y / H,
            y / H - x / W
        };
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

    std::string info() const {
        return "Position(" + std::to_string(x) 
            + ", " + std::to_string(y) + ")";
    }
};

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

    Coordinate to_grid() const {
        return {
            std::floor(x),
            std::floor(y)
        };
    }

    Position to_pixel() const {
        return {
            (x - y) * W * 0.5f,
            (x + y) * H * 0.5f
        };
    }

    operator sf::Vector2f() const {
        return {x, y};
    }

    operator sf::Vector2i() const {
        return {static_cast<int>(x), static_cast<int>(y)};
    }

    bool operator==(const Coordinate & other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Coordinate & other) const {
        return !operator==(other);
    }

    std::string info() const {
        return "Coordinate(" + std::to_string(x) 
            + ", " + std::to_string(y) + ")";
    }
};

#endif
