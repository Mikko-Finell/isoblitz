#include "map.hpp"
#include "common/serializable.hpp"
#include "common/helper.hpp"

Map::Map(const std::string & mapname, gfx::SpriteManager & spritem) {
    std::ifstream in{"../maps/" + mapname, std::ios::binary};
    load(in, tiles, spritem);
    in.close();
}

Tile * const Map::get_tile(const sf::Vector2f & coord) {
    for (auto & tile : tiles) {
        if (tile.coordinate() == coord) {
            return &tile;
        }
    }
    return nullptr;
}

std::vector<Tile *> 
Map::section(float left, float top, float width, float height) {
    std::vector<Tile *> vec;
    sf::FloatRect rect{left, top, width, height};
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
    ns[0] = get_tile({c.x, c.y - rows});
    ns[1] = get_tile({c.x + cols, c.y});
    ns[2] = get_tile({c.x, c.y + rows});
    ns[3] = get_tile({c.x - cols, c.y});
    return ns;
}

Cell * Map::get_cell(const sf::Vector2f & c) {
    if (Tile * tile = get_tile(c); tile != nullptr) {
        return tile->get_cell(c);
    }
    else {
        return nullptr;
    }
}

std::array<Cell *, DIRECTIONS> Map::neighbors(Cell * center) {
    std::array<Cell *, DIRECTIONS> ns;
    const auto c = center->coordinate();
    ns[0] = get_cell({c.x, c.y - 1});
    ns[1] = get_cell({c.x + 1, c.y});
    ns[2] = get_cell({c.x, c.y + 1});
    ns[3] = get_cell({c.x - 1, c.y});
    return ns;
}
