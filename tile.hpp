#ifndef __TILE__
#define __TILE__

#include <list>
#include <SFML/System.hpp>
#include "helper.hpp"

class Tile {
    Coordinate coordinate;

public:
    class Type {
	Coordinate spritecoords;
	bool blocked = false;
    public:
	Type(const Coordinate coords, bool blocked);
	bool operator==(const Type & other);
	bool operator!=(const Type & other);
    };
    std::list<Type> history;

    Tile();
    Tile(const Coordinate & coord);
    static Tile centered_at(const Position & pos);

    bool replace_with(const Type & other);
    void center_at(const Position & pos);
    bool operator==(const Tile & t) const;
    bool operator<(const Tile & t) const;
    void draw(std::vector<sf::Vertex> & vertices) const;

    std::string debug();
};

#endif
