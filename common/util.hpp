#ifndef util_hpp
#define util_hpp

#include "types.hpp"
#include "config.hpp"
#include "coordinate.hpp"
#include "sfml.hpp"
#include "stl.hpp"

namespace util {
inline UID generate_uid() {
    static UID id = 0;
    return ++id;
}

template<class T>
inline float distance(const T & a, const T & b) {
    const auto dx = a.x - b.x == 0 ? 0 : std::pow(a.x - b.x, 2);
    const auto dy = a.y - b.y == 0 ? 0 : std::pow(a.y - b.y, 2);
    return dx + dy == 0 ? 0 : std::sqrt(dx + dy);
}

template<int W = config::cellw, int H = config::cellh>
inline sf::Vector2f to_pixel(const sf::Vector2f & v) {
    float x = (v.x - v.y) * W * 0.5f;
    float y = (v.x + v.y) * H * 0.5f;
    return {x, y};
}

template<int W = config::cellw, int H = config::cellh>
inline sf::Vector2f to_map(const sf::Vector2f & v) {
    float x = v.x / W + v.y / H;
    float y = v.y / H - v.x / W;
    return {x, y};
}

template<int W = config::cellw, int H = config::cellh>
inline sf::Vector2f to_grid(const sf::Vector2f & v) {
    auto w = to_map<W, H>(v);
    w.x = std::floor(w.x);
    w.y = std::floor(w.y);
    return w;
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
} // util

#endif
