#ifndef coordinate_hpp
#define coordinate_hpp

#include <SFML/System/Vector2.hpp>
#include <string>
#include <cmath>
#include <cstdint>

/**
 * Position
 * Represents a visual position in the gameworld.
 */
class Position {
public:
    float x = 0;
    float y = 0;

    Position();
    Position(float _x, float _y);
    Position(int _x, int _y);
    Position(const sf::Vector2f & v);
    Position(const sf::Vector2i & v);

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

    float distance_to(const Position & other) const;
    Position operator-(const Position & other) const;
    Position operator+(const Position & other) const;
    Position operator*(const float z) const;
    Position operator/(const float z) const;
    void operator+=(const Position & other);
    operator sf::Vector2f() const;
    operator sf::Vector2i() const;
    bool operator==(const Position & other) const;
    bool operator!=(const Position & other) const;
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
    struct Hash {
        std::uint64_t operator()(const Coordinate<W, H> & coord) const;
    };

    float x = 0;
    float y = 0;

    Coordinate();
    Coordinate(float _x, float _y);
    Coordinate(int _x, int _y);
    Coordinate(const sf::Vector2f & v);
    Coordinate(const sf::Vector2i & v);
    Coordinate(const Position & pos);

    // snap to grid
    Coordinate to_grid() const;

    // convert to visual position, assuming
    // an isometric relationship between logical coordinates
    // and visual positions in the gameworld
    Position to_pixel() const;

    float distance_to(const Coordinate & other) const;
    operator sf::Vector2f() const;
    operator Position() const;
    operator sf::Vector2i() const;
    bool operator==(const Coordinate & other) const;
    bool operator!=(const Coordinate & other) const;

    void serialize(std::ostream & out) const;
    void deserialize(std::istream & in);
    std::string info() const;
};

#endif
