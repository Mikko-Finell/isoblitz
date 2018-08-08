#ifndef __TILE__
#define __TILE__

#include "common/mapobject.hpp"
#include "common/sprite.hpp"
#include <SFML/System.hpp>
#include <list>

class Tile : public MapObject {
public:
    gfx::Sprite blocked_sprite;

public:
    Tile(gfx::Manager & spritem);

    void set_coordinate(const sf::Vector2f & c) override;
    void set_blocked(bool b) override;
    void set_layer(int layer) override;

    void center_at(const sf::Vector2f & pos);
    bool operator==(const Tile & t) const;
    bool operator!=(const Tile & t) const { return !(*this == t); }
};

#endif
