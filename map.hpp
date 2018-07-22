#ifndef __MAP__
#define __MAP__

#include <vector>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "tile.hpp"

class Map {
    std::vector<Tile> tiles;

public:
    void create_at(const sf::Vector2f & pos);
    void remove_at(const sf::Vector2f & pos);
    void draw(std::vector<sf::Vertex> & vertices);
};

#endif
