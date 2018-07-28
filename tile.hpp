#ifndef __TILE__
#define __TILE__

#include <list>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "helper.hpp"
#include "serializable.hpp"

class Tile : public Serializable {
    Coordinate coord;
    Coordinate spritecoord;
    bool blocked = false;

    virtual void serialize(std::ostream & out) const override;
    void deserialize(std::istream & in) override;

public:
    static Tile empty_tile(const Coordinate & c = Coordinate{0,0});
    static Tile default_tile(const Coordinate & c = Coordinate{0,0});
    static Tile blocked_tile(const Coordinate & c = Coordinate{0,0});
    static Tile hl_tile(const Coordinate & c = Coordinate{0,0});

    Tile();
    Tile(const Coordinate & coord);
    static Tile from_position(const Position & pos);

    Coordinate coordinate() const;
    void center_at(const Position & pos);
    void set_coordinate(const Coordinate & c);
    void set_sprite(const Coordinate & c);
    void set_blocked(bool b);
    bool operator==(const Tile & t) const;
    bool operator!=(const Tile & t) const { return !(*this == t); }
    bool operator<(const Tile & t) const;
    bool is_empty_tile() const;
    bool is_blocked() const;
    virtual void draw(std::vector<sf::Vertex> & vertices) const;

    std::string debug() const;
    virtual ~Tile();
};

#endif
