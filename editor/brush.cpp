#include "brush.hpp"
#include "common/util.hpp"
#include <iostream>
#include <utility>

Brush::Brush(Map & m, gfx::SpriteManager & sm) : map(m), tile(sm), hltile(sm) {
    tile.set_sprite(sf::Vector2i(0, 128));
    hltile.set_sprite(sf::Vector2i(128, 128));
    hltile.set_layer(3);
}

void Brush::toggle_snap() {
    snap_to_grid = !snap_to_grid;
}

void Brush::on_paint_at(const sf::Vector2f & coord) {
    tile.set_coordinate(coord);
    map.create(tile);
}

void Brush::on_paint() {
    map.create(tile);
}

void Brush::on_erase() {
    map.remove(tile.coordinate());
}

void Brush::on_update_mousepos(const sf::Vector2f & pos) {
    sf::Vector2f coord;
    if (snap_to_grid) {
        coord = util::to_grid<TILEW,TILEH>(pos);
        coord.x *= COLS_PER_TILE;
        coord.y *= ROWS_PER_TILE;
    }
    else {
        coord = util::to_grid(pos);
        // center of tile:
        coord.x -= COLS_PER_TILE / 2;
        coord.y -= ROWS_PER_TILE / 2;
    }
    tile.set_coordinate(coord);
    hltile.set_coordinate(tile.coordinate());
}

void Brush::on_setsprite(const sf::Vector2i & coord) {
    tile.set_sprite(sf::Vector2i(coord));
}

void Brush::on_setblocked(bool b) {
    tile.set_blocked(b);
    std::cout << "Brush set to " << (tile.is_blocked() ? "blocked" : "open")
        << std::endl;
}
