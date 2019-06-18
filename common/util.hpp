#ifndef util_hpp
#define util_hpp

#include "config.hpp"
#include "coordinate.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <fstream>
#include <cmath>

namespace util {
using UID = unsigned long long;
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

// TODO easy
// create an object that handles write and read 
// instead of these functions
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
