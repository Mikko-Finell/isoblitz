#ifndef __BRUSH__
#define __BRUSH__

#include "ui.hpp"
#include "helper.hpp"
#include "tile.hpp"
#include "coordinate.hpp"
#include "listener.hpp"
#include "emitter.hpp"

class Brush : public Listener, public Emitter, private Tile {
    Coordinate current_coord;

public:
    Tile::Type current_tile;
    Tile::Type empty_tile;
    Tile::Type default_tile;

    Brush();
    Coordinate coordinate() const;
    void draw(std::vector<sf::Vertex> & vertices) const override;

    void recvevent(Event event) override;
};

#endif
