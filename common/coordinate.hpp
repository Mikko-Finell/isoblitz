#ifndef coordinate_hpp
#define coordinate_hpp

#include "config.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
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

    class Region {
    public:
        const float x=0.0, y=0.0, width=0.0, height=0.0;
        Region(float _x=0.0, float _y=0.0, float _width=0.0, float _height=0.0);
        Region(const sf::FloatRect & rect);
        Position top_left() const;
        Position top_right() const;
        Position bottom_left() const;
        Position bottom_right() const;
        bool intersects(const Region & other) const;
        bool contains(const Position & coord) const;
        std::string info() const;
    };

    Position();
    Position(float _x, float _y);
    Position(int _x, int _y);
    Position(const sf::Vector2f & v);
    Position(const sf::Vector2i & v);

    // convert pixel position to logical coordinates
    // using a grid with cells of width W and height H
    // and assuming isometric relationship between 
    // visual/logical coordinates
    template<int W=config::cellw, int H=config::cellh>
    sf::Vector2f to_logic() const {
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
    std::string info() const;
};

/**
 * Coordinate
 * Represents a coordinate in game logic.
 * W and H params signify the width and height of grid cells.
 * note: Why should Coordinate need to know W and H?
 */
namespace impl {
template<int W, int H>
class Coordinate {
public:
    struct Hash {
        std::uint64_t operator()(const Coordinate<W, H> & coord) const;
    };
    class Region {
    public:
        const float x=0.0, y=0.0, width=0.0, height=0.0;
        Region(float _x=0.0, float _y=0.0, float _width=0.0, float _height=0.0);
        Region(const sf::FloatRect & rect);
        Coordinate top_left() const;
        Coordinate top_right() const;
        Coordinate bottom_left() const;
        Coordinate bottom_right() const;
        bool intersects(const Region & other) const;
        bool contains(const Coordinate & coord) const;
        std::string info() const;
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

    std::string info() const;
};
} // impl

using Coordinate = impl::Coordinate<config::cellw, config::cellh>;

#endif
