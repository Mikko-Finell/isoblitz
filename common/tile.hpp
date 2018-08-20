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

class Tile {
    int layer = TILE_LAYER;
    tile_id_t id;
    coord_t coord;
    Sprite sprite;

public:
    Tile(tile_id_t id = 0);
    Tile(std::istream & in, RenderSystem & rs);
    void init(RenderSystem & rs);
    void set_spritedata(const SpriteData & spritedata);
    void set_coordinate(int x, int y);
    void set_coordinate(const coord_t & c);
    void serialize(std::ostream & out) const;
    void deserialize(std::istream & in);

    sf::Vector2f get_position() const;
    coord_t get_coordinate() const;
    tile_id_t get_id() const;
    SpriteData get_spritedata() const;
};

#endif
