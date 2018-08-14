#ifndef __TILE__
#define __TILE__

#include "common/mapobject.hpp"
#include "common/util.hpp"

class Tile;

class Cell {
    sf::Vector2f coord;
    Tile * tile;

public:
    Cell(Tile * t = nullptr);
    sf::Vector2f get_position() const;
    void set_tile(Tile * t);
    sf::Vector2f get_coordinate() const;
    void set_coordinate(const sf::Vector2f & c);
};

class Map;

class Tile : public map::MapObject {
    std::array<Cell, COLS_PER_TILE * ROWS_PER_TILE> cells;

public:
    Tile(gfx::SpriteManager & spritem);
    void set_coordinate(const sf::Vector2f & c);
    Cell * get_cell_globalcoord(int x, int y, const std::string & why);
    Cell * get_cell_localcoord(int x, int y, const std::string & why);
    bool contains(const sf::Vector2f & c) const;
};

#endif 
