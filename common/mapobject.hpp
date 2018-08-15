#ifndef __MAPOBJ__
#define __MAPOBJ__

#include "sprite.hpp"
#include "util.hpp"
#include <SFML/System/Vector2.hpp>
#include <cassert>

namespace map {

class MapObject {
    gfx::Sprite sprite;
    sf::Vector2f coord;
    bool blocked = false;
    int z = 0;

public:
    virtual ~MapObject();
    MapObject(gfx::SpriteManager & spritem);

    virtual void set_coordinate(const sf::Vector2f & c);
    virtual void set_sprite(const sf::Vector2i & c);
    virtual void set_blocked(bool b);
    virtual void move(const sf::Vector2f & offset);
    virtual void set_layer(int layer);

    void serialize(std::ostream & out) const;
    void deserialize(std::istream & in);

    sf::Vector2f coordinate() const;
    const gfx::Sprite & get_sprite() const;
    int get_layer() const;
    bool is_blocked() const;
};

template<class T>
inline std::pair<int, int> normalize(std::vector<T> & objs) {
    static_assert(std::is_base_of<MapObject, T>::value);

    float left = 0, right = 0, top = 0, bottom = 0;
    if (objs.empty() == false) {
        auto initc = objs.front().coordinate();
        left = initc.x;
        right = initc.x;
        top = initc.y;
        bottom = initc.y;
    }
    std::size_t objcount = 0;
    for (auto & obj : objs) {
        ++objcount;
        // update bounds
        auto vec2f = obj.coordinate();
        left = std::min(vec2f.x, left);
        right = std::max(vec2f.x, right);
        top = std::min(vec2f.y, top);
        bottom = std::max(vec2f.y, bottom);
    }
    sf::Vector2f offset{-left, -top};
    for (auto & obj : objs) {
        obj.move(offset);
    }

    std::size_t width = 0;
    std::size_t height = 0;

    if (objcount) {
        width = std::abs(right - left) + 1;
        height = std::abs(bottom - top) + 1;
    }
    return {width, height};
}

template<class T>
inline std::pair<int, int> save(std::ofstream & out, std::vector<T> & objs) {
    static_assert(std::is_base_of<MapObject, T>::value);

    const auto [width, height] = normalize(objs);
    const std::size_t objcount = objs.size();
    const int editor_version = EDITOR_VERSION;

    util::write(editor_version, out);
    util::write(objcount, out);
    util::write(width, out);
    util::write(height, out);

    for (auto & obj : objs) {
        obj.serialize(out);
    }
    return {width, height};
}

template<class T>
std::pair<int,int> 
load(std::ifstream & in, std::vector<T> & objs, gfx::SpriteManager & spritem) {
    static_assert(std::is_base_of<MapObject, T>::value);

    int map_version;
    std::size_t objcount;
    int width, height;

    util::read(map_version, in);
    assert(map_version == EDITOR_VERSION);

    util::read(objcount, in);
    util::read(width, in);
    util::read(height, in);

    objs.resize(objcount, spritem);
    for (auto & obj : objs) {
        obj.deserialize(in);
    }
    return {width, height};
}

} // map

#endif
