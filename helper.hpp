#ifndef __HELPER__
#define __HELPER__

#define WINW 800
#define WINH 800
#define TILEW 128.0f
#define TILEH 64.0f
#define HALFW (TILEW / 2)
#define HALFH (TILEH / 2)

#include <cmath>
#include "position.hpp"
#include "coordinate.hpp"

inline Position logic_to_pixel(const Coordinate & v) {
    float x = (v.x - v.y) * HALFW;
    float y = (v.x + v.y) * HALFH;
    return Position{x, y};
}

inline Coordinate pixel_to_logic(const Position & v) {
    float x = (v.x / HALFW + v.y / HALFH) / 2;
    float y = (v.y / HALFH - v.x / HALFW) / 2;
    return Coordinate{x, y};
}

inline Position snap_to_grid(const Position & v) {
    auto w = pixel_to_logic(v);
    w.x = std::floor(w.x);
    w.y = std::floor(w.y);
    return logic_to_pixel(w);
}

inline Coordinate tile_center_at(const Position & v) {
    auto u = v;
    u.x -= HALFW;
    //v.y -= HALFH;
    auto w = snap_to_grid(u);
    return pixel_to_logic(w);
}

/*
void user_click(sf::Vector2f v, std::vector<Tile> & tiles) {
    tiles.push_back(Tile{tile_center_at(v)});
}
*/

#endif
