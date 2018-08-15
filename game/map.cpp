#include "map.hpp"

Map::Map(gfx::SpriteManager & sm) : spritem(sm) {
}

void Map::load(const std::string & mapname) {
    std::ifstream in{"../maps/" + mapname, std::ios::binary};
    int width, height;
    std::tie(width, height) = map::load(in, tiles, spritem);
    in.close();
    signal.map_loaded(width, height);
}

Tile * const Map::get_tile(const sf::Vector2f & coord) {
    for (auto & tile : tiles) {
        if (tile.contains(coord)) {
            return &tile;
        }
    }
    return nullptr;
}

std::vector<Tile *> Map::section(const sf::FloatRect & rect) {
    std::vector<Tile *> vec;
    for (auto & tile : tiles) {
        if (rect.contains(tile.coordinate())) {
            vec.push_back(&tile);
        }
    }
    return vec;
}

std::array<Tile *, DIRECTIONS> Map::neighbors(Tile * center) {
    std::array<Tile *, DIRECTIONS> ns;
    const auto c = center->coordinate();
    ns[0] = get_tile({c.x, c.y - ROWS_PER_TILE});
    ns[1] = get_tile({c.x + COLS_PER_TILE, c.y});
    ns[2] = get_tile({c.x, c.y + ROWS_PER_TILE});
    ns[3] = get_tile({c.x - COLS_PER_TILE, c.y});
    return ns;
}

//std::pair<int, int> Map::size() const {
    //return {width, height};
//}
/*
cell_t * Map::get_cell(int x, int y) {
    auto tile = get_tile(sf::Vector2f(x, y));
    if (tile != nullptr) {
        return tile->get_cell_globalcoord(x, y, "map::get_cell");
    }
    return nullptr;
}
*/
