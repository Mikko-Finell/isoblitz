#ifndef tilefactory_hpp
#define tilefactory_hpp

#include "tile.hpp"
#include "coordinate.hpp"
#include "util.hpp"
#include "spritefactory.hpp"
#include "rendersystem.hpp"
#include <unordered_map>
#include <vector>

/**
 * TileFactory
 * Produces tiles from ids.
 * Important note: Tile id are generated automatically so
 * adding new tiles may break existing maps.
 * TODO consider some way to fix this
 */
class TileFactory {
    std::unordered_map<Tile::ID, SpriteImpl> sprites;
    SpriteFactory & spritef;
    RenderSystem & default_rs;

public:
    TileFactory(SpriteFactory & sf, RenderSystem & rs);
    Tile create(RenderSystem & rs, Tile::ID) const;
    Tile create(Tile::ID) const;
    std::vector<Tile::ID> get_all() const;
    void deserialize(IOReader & in);
};

#endif
