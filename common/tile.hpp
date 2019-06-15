#ifndef tile_hpp
#define tile_hpp

#include "coordinate.hpp"
#include "sprite.hpp"
#include "rendersystem.hpp"
#include "util.hpp"
#include <SFML/System/Vector2.hpp>
#include <fstream>

using tile_id_t = std::size_t;
using coord_t = Coordinate<TILEW, TILEH>;

/**
 * Tile
 * Represents an isometric tile in the game world.
 */
class Tile {
    int layer = TILE_LAYER;
    tile_id_t id;
    coord_t coord;

public:
    // TODO easy
    // could be private?
    Sprite sprite;

    Tile(tile_id_t id = 0);
    Tile(std::istream & in, RenderSystem & rs);
    void set_coordinate(int x, int y);
    void set_coordinate(const coord_t & c);
    void serialize(std::ostream & out) const;
    void deserialize(std::istream & in);

    sf::Vector2f get_position() const;
    coord_t get_coordinate() const;
    tile_id_t get_id() const;
};

#endif
