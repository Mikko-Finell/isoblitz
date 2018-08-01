#ifndef __BRUSH__
#define __BRUSH__

#include "common/coordinate.hpp"
#include "common/drawable.hpp"
#include "common/observer.hpp"
#include "tile.hpp"
#include "map.hpp"
#include "ui.hpp"

class Brush : public Observer, public Drawable {
    Tile tile;
    Map & map;

public:
    Brush(Map & m);
    Coordinate coordinate() const;
    void draw(VertexArray & vertices) const override;

    void on_paint();
    void on_erase();
    void on_update_mousepos(const Position & pos);
    void on_setsprite(const Coordinate & coord);
    void on_setblocked(bool b);
};

#endif
