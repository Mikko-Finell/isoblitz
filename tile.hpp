#ifndef __TILE__
#define __TILE__

#include <list>
#include <SFML/System.hpp>

class Tile {
    sf::Vector2f coordinate;

public:
    class Type {
	sf::Vector2f spritecoords;
	bool blocked = false;
    public:
	Type(const sf::Vector2f coords, bool blocked);
	bool operator==(const Type & other);
	bool operator!=(const Type & other);
    };
    std::list<Type> history;

    Tile();
    Tile(const sf::Vector2f & coord);
    static Tile centered_at(const sf::Vector2f & pos);

    bool replace_with(const Type & other);
    void center_at(const sf::Vector2f & pos);
    bool operator==(const Tile & t) const;
    bool operator<(const Tile & t) const;
    void draw(std::vector<sf::Vertex> & vertices) const;

    std::string debug();
};

#endif
