#ifndef __TILE__
#define __TILE__

#include "common/mapobject.hpp"

static constexpr int cols = 4;
static constexpr int rows = 4;

class Cell {
    std::array<Cell *, DIRECTIONS> neighbors;
    sf::Vector2f coord;

public:
    Cell() {
        neighbors.fill(nullptr);
    }

    sf::Vector2f coordinate() const {
        return coord;
    }

    void set_coordinate(const sf::Vector2f & c) {
        coord = c;
    }

    void set_neighbor(int x, int y, Cell * cell) {
        if (y == -1) {
            neighbors[0] = cell;
        }
        else if (x == 1) {
            neighbors[1] = cell;
        }
        else if (y == 1) {
            neighbors[2] = cell;
        }
        else {
            neighbors[3] = cell;
        }
    }
};

class Tile : public MapObject {
    std::array<Cell, cols * rows> cells;
    std::array<Tile *, DIRECTIONS> neighbors;

public:
    Tile(gfx::SpriteManager & spritem);
    void set_coordinate(const sf::Vector2f & c) override;
    Cell * get_cell(const sf::Vector2f & c);
    Cell * get_cell(int x, int y);
    bool contains(const sf::Vector2f & c) const;
    void set_neighbor(int x, int y, Tile * tile);
};

#endif 
