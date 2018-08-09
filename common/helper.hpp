#ifndef __HELPER__
#define __HELPER__

#define WINW 1200
#define WINH 1000
#define TILEW 128.0f
#define TILEH 64.0f
#define SPRIW 128
#define SPRIH 128
#define HALFW (TILEW * 0.5f)
#define HALFH (TILEH * 0.5f)
#define DIRECTIONS 4
#define EDITOR_VERSION 0

#include <SFML/System/Vector2.hpp>
#include <cmath>

inline sf::Vector2f logic_to_pixel(const sf::Vector2f & v) {
    float x = (v.x - v.y) * HALFW;
    float y = (v.x + v.y) * HALFH;
    return {x, y};
}

inline sf::Vector2f pixel_to_logic(const sf::Vector2f & v) {
    float x = 0.5f * (v.x / HALFW + v.y / HALFH);
    float y = 0.5f * (v.y / HALFH - v.x / HALFW);
    return {x, y};
}

inline sf::Vector2f to_grid(const sf::Vector2f & v) {
    auto w = pixel_to_logic(v);
    w.x = std::floor(w.x);
    w.y = std::floor(w.y);
    return w;
}

#endif
