#ifndef __HELPER__
#define __HELPER__

#define WINW 1200
#define WINH 1000
#define TILEW 128.0f
#define TILEH 64.0f
#define SPRIW 128
#define SPRIH 128
#define HALFW (TILEW / 2)
#define HALFH (TILEH / 2)

#include <cmath>
#include <SFML/System/Vector2.hpp>

inline sf::Vector2f logic_to_pixel(const sf::Vector2f & v) {
    float x = (v.x - v.y) * HALFW;
    float y = (v.x + v.y) * HALFH;
    return {x, y};
}

inline sf::Vector2f pixel_to_logic(const sf::Vector2f & v) {
    float x = (v.x / HALFW + v.y / HALFH) / 2;
    float y = (v.y / HALFH - v.x / HALFW) / 2;
    return sf::Vector2f(x, y);
}

inline sf::Vector2f snap_to_grid(const sf::Vector2f & v) {
    auto w = pixel_to_logic(v);
    w.x = std::floor(w.x);
    w.y = std::floor(w.y);
    return logic_to_pixel(w);
}

inline sf::Vector2f tile_center_at(const sf::Vector2f & v) {
    auto u = v;
    u.x -= HALFW;
    //v.y -= HALFH;
    auto w = snap_to_grid(u);
    return pixel_to_logic(w);
}

#endif
