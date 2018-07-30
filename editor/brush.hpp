#ifndef __BRUSH__
#define __BRUSH__

#include "common/coordinate.hpp"
#include "common/drawable.hpp"
#include "listener.hpp"
#include "emitter.hpp"
#include "ui.hpp"
#include "tile.hpp"

class Brush : public Listener, public Emitter, public Drawable {
    Tile tile;

public:
    Brush();
    Coordinate coordinate() const;
    void draw(VertexArray & vertices) const override;
    void recvevent(const Event & event) override;
};

#endif
