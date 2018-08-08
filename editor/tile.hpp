#ifndef __TILE__
#define __TILE__

#include <list>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "common/serializable.hpp"
#include "common/sprite.hpp"

class Tile : public Serializable {
public:
    sf::Vector2i spritecoord;
    sf::Vector2f coord;
    bool blocked = false;
    gfx::Sprite main_sprite, blocked_sprite;
    int z = 0;

    void serialize(std::ostream & out) const override;
    void deserialize(std::istream & in) override;

public:
    Tile(gfx::Manager & spritem);

    sf::Vector2f coordinate() const;
    void center_at(const sf::Vector2f & pos);
    void set_coordinate(const sf::Vector2f & c);
    void set_sprite(const sf::Vector2i & c);
    void set_blocked(bool b);
    void move(const sf::Vector2f & offset);
    void set_layer(int layer);
    int get_layer() const;
    bool operator==(const Tile & t) const;
    bool operator!=(const Tile & t) const { return !(*this == t); }
    bool is_empty_tile() const;
    bool is_blocked() const;

    std::string debug() const;
    virtual ~Tile() {}
};

#endif
