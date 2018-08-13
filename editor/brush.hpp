#ifndef __BRUSH__
#define __BRUSH__

#include "tile.hpp"
#include "map.hpp"
#include "common/observer.hpp"
#include "common/sprite.hpp"

class Brush : public Observer {
    Tile tile;
    Tile hltile;
    Map & map;
    bool snap_to_grid = true;

public:
    Brush(Map & m, gfx::SpriteManager & sm);

    void toggle_snap();
    void on_paint_at(const sf::Vector2f & coord);
    void on_paint();
    void on_erase();
    void on_update_mousepos(const sf::Vector2f & pos);
    void on_setsprite(const sf::Vector2i & coord);
    void on_setblocked(bool b);
};

#endif
