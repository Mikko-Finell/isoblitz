#include "brush.hpp"
#include "common/util.hpp"
#include <iostream>
#include <utility>

Brush::Brush(Map & m, RenderSystem & rs) : map(m), tile(rs), hltile(rs) {
    tile.sprite.set_spritecoord(0, 128)
               .set_layer(TILE_INDICATOR_LAYER);
    hltile.sprite.set_spritecoord(128, 128)
                 .set_layer(TILE_INDICATOR_LAYER + 1);
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
    tile.sprite.set_spritecoord(coord);
}

void Brush::on_setblocked(bool b) {
    tile.set_blocked(b);
    std::cout << "Brush set to " << (tile.is_blocked() ? "blocked" : "open")
        << std::endl;
}
