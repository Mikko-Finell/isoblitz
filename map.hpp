#ifndef __MAP__
#define __MAP__

#include <list>
#include <vector>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "tile.hpp"
#include "listener.hpp"
#include "serializable.hpp"

class Map : public Listener, public Serializable {
    std::list<Tile> tiles;
    std::list<Tile*> history;

    void serialize(std::ostream & out) override;
    void deserialize(std::istream & in) override;

public:
    void undo();
    void create(const Tile::Type & type, const Coordinate & coord);
    void remove(const Coordinate & coord);
    void draw(std::vector<sf::Vertex> & vertices);
    void recvevent(const Event & event) override;
};

#endif
