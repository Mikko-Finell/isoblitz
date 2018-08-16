#ifndef __MAP__
#define __MAP__

#include "tile.hpp"
#include "map.hpp"
#include "common/observer.hpp"
#include <vector>

class Map {
    std::vector<Tile> tiles;
    SpriteManager & spritem;
    //int width = 0, height = 0;

public:
    struct {
        Signal<int, int> map_loaded;
    } signal;

    Map(SpriteManager & spritem);

    void load(const std::string & mapname);

    Tile * const get_tile(const sf::Vector2f & coord);
    std::vector<Tile*> section(const sf::FloatRect & rect);
    std::array<Tile *, DIRECTIONS> neighbors(Tile * center);
    //std::pair<int, int> size() const;
};

#endif
