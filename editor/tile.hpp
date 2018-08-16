#ifndef __TILE__
#define __TILE__

#include "common/mapobject.hpp"
#include "common/sprite.hpp"
#include <SFML/System.hpp>
#include <list>

class Tile : public map::MapObject {
public:
    Sprite blocked_sprite;

public:
    Tile(RenderSystem & rs);

    void set_coordinate(const sf::Vector2f & c) override;
    void set_blocked(bool b) override;
    void set_layer(int layer) override;

    bool intersects(const Tile & other) const;
    bool operator==(const Tile & t) const;
    bool operator!=(const Tile & t) const { return !(*this == t); }
};

#endif
