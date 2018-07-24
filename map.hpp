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
    void create_at(const Coordinate & coord);
    void remove_at(const Coordinate & coord);
    void draw(std::vector<sf::Vertex> & vertices);
};

#endif
