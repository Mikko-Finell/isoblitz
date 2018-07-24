#ifndef __MAP__
#define __MAP__

#include <list>
#include <vector>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "tile.hpp"

class Map {
    std::list<Tile> tiles;
    std::list<Tile*> history;

public:
    void undo();
    void create(const Tile::Type & type, const Coordinate & coord);
    void remove(const Coordinate & coord);
    void draw(std::vector<sf::Vertex> & vertices);
};

#endif
