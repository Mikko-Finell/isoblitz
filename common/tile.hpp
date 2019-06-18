#ifndef tile_hpp
#define tile_hpp

#include "coordinate.hpp"
#include "sprite.hpp"
#include "rendersystem.hpp"
#include "util.hpp"
#include <SFML/System/Vector2.hpp>
#include <fstream>

/**
 * Tile
 * Represents an isometric tile in the game world.
 */
class Tile {
public:
    using ID = std::size_t;
    using Coord = Coordinate<config::tilew, config::tileh>;

    Sprite sprite;

    Tile(ID id = 0);
    void set_coordinate(int x, int y);
    void set_coordinate(const Coord & c);
    void serialize(std::ostream & out) const;
    void deserialize(std::istream & in);

    sf::Vector2f get_position() const;
    Coord get_coordinate() const;
    ID get_id() const;

private:
    int layer = config::tile_layer;
    ID id;
    Coord coord;
};

using Cell = Coordinate<config::cellw, config::cellh>;

#endif
