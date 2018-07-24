#ifndef __BRUSH__
#define __BRUSH__

#include "coordinate.hpp"
#include "ui.hpp"
#include "helper.hpp"
#include "tile.hpp"

class Brush : private Tile {
    UI * ui = nullptr;

public:
    Tile::Type current_tile;
    Tile::Type empty_tile;
    Tile::Type default_tile;

    Brush();
    void attach(UI & _ui);
    Coordinate coordinate() const;
    void draw(std::vector<sf::Vertex> & vertices) const override;
};

#endif
