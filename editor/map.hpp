#ifndef __MAP__
#define __MAP__

#include <list>
#include <stack>
#include <vector>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "tile.hpp"
#include "listener.hpp"
#include "serializable.hpp"

class Map : public Listener, public Serializable {
    std::list<std::stack<Tile>> tiles;
    std::list<std::stack<Tile>*> history;
    const std::string extension = ".bulletmap";

    void serialize(std::ostream & out) const override;
    void deserialize(std::istream & in) override;

public:
    std::string name = "tmp";
    std::string filename() const {
        return name + extension;
    }

    void undo();
    void create(const Tile & tile);
    void remove(const Coordinate & coord);
    void draw(std::vector<sf::Vertex> & vertices);
    void recvevent(const Event & event) override;
};

#endif
