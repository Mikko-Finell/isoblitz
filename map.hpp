#ifndef __MAP__
#define __MAP__

#include <list>
#include <vector>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "tile.hpp"
#include "listener.hpp"

class Map : public Listener {
    std::list<Tile> tiles;
    std::list<Tile*> history;

public:
    void undo();
    void create(const Tile::Type & type, const Coordinate & coord);
    void remove(const Coordinate & coord);
    void draw(std::vector<sf::Vertex> & vertices);
    void recvevent(Event event) override;
};

#endif
