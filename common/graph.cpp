#include "graph.hpp"

Graph::~Graph() {
}

Graph::Graph(const std::list<Tile> & tiles) {
    for (auto & tile : tiles) {
        for (const Coordinate & coord : tile.get_region().to_list()) {
            cell_map[coord] = true;
        }
        auto & t = tile_map[tile.get_coordinate()] = tile;
        t.sprite.clear();
    }
}

Tile & Graph::get_tile_at(const Coordinate & coord) {
    return tile_map.at(coord);
}

Tile & Graph::get_tile_containing(const Coordinate & coord) {
    for (auto & pair : tile_map) {
        Tile & tile = pair.second;
        if (tile.contains(coord)) {
            return tile;
        }
    }
    throw std::out_of_range{"No tile contains " + coord.info()};
}

std::list<const Tile *> Graph::get_tiles(const Coordinate::Region & region) {
    std::list<const Tile *> ts;
    for (auto & pair : tile_map) {
        const Tile & tile = pair.second;
        if (region.intersects(tile.get_region())) {
            ts.push_back(&tile);
        }
    }
    return ts;
}

std::vector<Tile *> Graph::tile_neighbors(const Coordinate & coordinate) {
    throw std::logic_error{"Graph::tile_neighbors not implemented."};
}

std::vector<Coordinate> Graph::neighbors(const Coordinate & coordinate) {
    static constexpr int directions[] = { -1, 0, 1 };
    std::vector<Coordinate> cs;
    cs.reserve(8);
    for (const int x : directions) {
        for (const int y : directions) {
            if (x == 0 and y == 0) {
                continue;
            }
            else {
                Coordinate neighbor{coordinate.x + x, coordinate.y + y};
                if (cell_map[neighbor] == true) {
                    cs.push_back(neighbor);
                }
            }
        }
    }
    return cs;
}
