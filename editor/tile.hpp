#ifndef __TILE__
#define __TILE__

#include <list>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "common/serializable.hpp"
#include "common/drawable.hpp"

class Tile : public Serializable, public Drawable {
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
    void move(const Coordinate & offset);
    Tile moved(const Coordinate & offset) const;
    bool operator==(const Tile & t) const;
    bool operator!=(const Tile & t) const { return !(*this == t); }
    bool operator<(const Tile & t) const;
    bool is_empty_tile() const;
    bool is_blocked() const;
    virtual void draw(VertexArray & vertices) const override;

    std::string debug() const;
    virtual ~Tile();
};

#endif
