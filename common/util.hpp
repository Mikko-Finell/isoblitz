#ifndef util_hpp
#define util_hpp

#define WINW 800
#define WINH 800
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
#define TILE_INDICATOR_LAYER 1
#define ENTITY_LAYER 2
#define ENTITY_INDICATOR_LAYER 3
#define UI_LAYER 4

#include "coordinate.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <fstream>
#include <cmath>

using cell_t = Coordinate<CELLW, CELLH>;
using type_id_t = std::string;
using uuid_t = std::size_t;

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

template<typename T>
inline void write(const T & t, std::ostream & out) {
    out.write(reinterpret_cast<const char*>(&t), sizeof(t));
}

template<typename T>
inline void read(T & t, std::istream & in) {
    in.read(reinterpret_cast<char*>(&t), sizeof(t));
}

inline void serialize_std_string(const std::string & str, std::ostream & out) {
    write(str.length(), out);
    out << str;
}

inline std::string deserialize_std_string(std::istream & in) {
    std::string::size_type length;
    read(length, in);
    std::string str;
    str.resize(length);
    in.read(str.data(), length);
    return str;
}

template<class T>
inline std::string rect_to_str(const sf::Rect<T> & rect) {
    std::string str;
    str += std::to_string(rect.left) + ", ";
    str += std::to_string(rect.top) + ", ";
    str += std::to_string(rect.width) + ", ";
    str += std::to_string(rect.height);
    return str;
}

template<class T>
inline std::string vec_to_str(const sf::Vector2<T> & vec) {
    std::string str;
    str += std::to_string(vec.x) + ", ";
    str += std::to_string(vec.y);
    return str;
}
} // util

#endif
