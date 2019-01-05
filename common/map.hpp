#ifndef map_bpp
#define map_bpp

#include "tile.hpp" 
#include "tilefactory.hpp"
#include "rendersystem.hpp"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

/**
 * Map
 * Represents an interface to the game world/map.
 */
class Map {
    // TODO consider if these should be hardcoded
    // or loaded from database
    const std::string extension = ".bulletmap";
    const std::string mapdir = "../maps/";
    std::string name = "tmp";

    std::vector<Tile> tiles;
    TileFactory & tilef;
    RenderSystem & render;

public:
    Map(RenderSystem & rs, TileFactory & tf);
    void add_tile(const tile_id_t & id, const coord_t & coord);
    void remove_tile(const coord_t & coord);

    inline void clear() {
        tiles.clear();
    }

    inline std::string filename() const {
        return mapdir + name + extension;
    }
    void serialize(std::ostream & out) const;
    void deserialize(std::istream & in);
};

#endif
