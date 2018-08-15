#ifndef __UTIL__
#define __UTIL__

#define WINW 1200
#define WINH 1000
#define TILEW 128
#define TILEH TILEW / 2
#define COLS_PER_TILE 8
#define ROWS_PER_TILE 8
#define CELLW TILEW / COLS_PER_TILE
#define CELLH TILEH / ROWS_PER_TILE
#define SPRIW 128
#define SPRIH 128
#define HALFW (TILEW * 0.5f)
#define HALFH (TILEH * 0.5f)
#define DIRECTIONS 4
#define EDITOR_VERSION 0
#define TILE_LAYER 0
#define ENTITY_LAYER 1

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <fstream>
#include <cmath>

namespace util {
template<int W = CELLW, int H = CELLH>
inline sf::Vector2f to_pixel(const sf::Vector2f & v) {
    float x = (v.x - v.y) * W * 0.5f;
    float y = (v.x + v.y) * H * 0.5f;
    return {x, y};
}

template<int W = CELLW, int H = CELLH>
inline sf::Vector2f to_map(const sf::Vector2f & v) {
    float x = v.x / W + v.y / H;
    float y = v.y / H - v.x / W;
    return {x, y};
}

template<int W = CELLW, int H = CELLH>
inline sf::Vector2f to_grid(const sf::Vector2f & v) {
    auto w = to_map<W, H>(v);
    w.x = std::floor(w.x);
    w.y = std::floor(w.y);
    return w;
}

inline void center_window(sf::RenderWindow & window) {
    window.setPosition(sf::Vector2i{1920/2-WINW/2, 1080/2-WINH/2});
}

template<typename T>
inline void write(const T & t, std::ostream & out) {
    out.write(reinterpret_cast<const char*>(&t), sizeof(t));
}

template<typename T>
inline void read(T & t, std::istream & in) {
    in.read(reinterpret_cast<char*>(&t), sizeof(t));
}
} // util

#endif
