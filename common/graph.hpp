#ifndef graph_hpp
#define graph_hpp

#include "tile.hpp"
#include <list>
#include <vector>
#include <unordered_map>
#include <unordered_set>

class Graph {
    struct Hash {
        std::uint64_t operator()(const Tile & tile) const {
            return Coordinate::Hash::generate(tile.get_coordinate());
        }
    };
    std::unordered_map<Coordinate, std::vector<Tile *>, Coordinate::Hash> tile_connectome;
    std::unordered_map<Coordinate, Tile, Coordinate::Hash> tile_map;
    std::unordered_map<Coordinate, bool, Coordinate::Hash> cell_map;

public:
    ~Graph();
    Graph(const std::list<Tile> & tiles = {});

    Tile & get_tile_at(const Coordinate & coord);
    Tile & get_tile_containing(const Coordinate & coord);
    std::list<const Tile *> get_tiles(const Coordinate::Region & region);
    std::vector<Tile *> tile_neighbors(const Coordinate & coordinate);
    std::vector<Coordinate> neighbors(const Coordinate & coordinate);
};

#endif
