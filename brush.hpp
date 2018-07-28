#ifndef __BRUSH__
#define __BRUSH__

#include "ui.hpp"
#include "helper.hpp"
#include "tile.hpp"
#include "coordinate.hpp"
#include "listener.hpp"
#include "emitter.hpp"

class Brush : public Listener, public Emitter, private Tile {
    Tile tile;

public:
    Brush();
    Coordinate coordinate() const;
    void draw(std::vector<sf::Vertex> & vertices) const override;

    void recvevent(const Event & event) override;
};

#endif
