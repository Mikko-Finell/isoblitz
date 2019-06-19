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

    Sprite sprite;

    Tile(ID id = 0);
    void set_coordinate(int x, int y);
    void set_coordinate(const Coordinate & c);
    sf::Vector2f get_position() const;
    Coordinate get_coordinate() const;
    ID get_id() const;

private:
    int layer = config::tile_layer;
    ID id;
    Coordinate coord;

    void serialize(std::ostream & out) const;
    void deserialize(std::istream & in);
};

#endif
