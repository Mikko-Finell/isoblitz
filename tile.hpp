#ifndef __TILE__
#define __TILE__

#include <SFML/System.hpp>

class Tile {
    sf::Vector2f coordinate;

public:
    Tile();
    Tile(const sf::Vector2f & coord);
    static Tile centered_at(const sf::Vector2f & pos);

    void center_at(const sf::Vector2f & pos);
    bool operator==(const Tile & t) const;
    bool operator<(const Tile & t) const;
    void draw(std::vector<sf::Vertex> & vertices) const;
};

#endif
