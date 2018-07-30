#ifndef __MAP__
#define __MAP__

#include <list>
#include <stack>
#include <vector>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "common/serializable.hpp"
#include "event.hpp"
#include "listener.hpp"
#include "tile.hpp"

class Map : public Listener, public Serializable {
    std::list<std::stack<Tile>> tiles;
    std::list<std::stack<Tile>*> history;
    const std::string extension = ".bulletmap";

    void serialize(std::ostream & out) const override;
    void deserialize(std::istream & in) override;

public:
    std::string name = "tmp";
    std::string filename() const {
        return "../maps/" + name + extension;
    }

    void undo();
    void create(const Tile & tile);
    void remove(const Coordinate & coord);
    void draw(VertexArray & vertices);
    void recvevent(const Event & event) override;
};

#endif
