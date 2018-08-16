#include "mapobject.hpp"
#include "util.hpp"
#include <cassert>

namespace map {

void MapObject::serialize(std::ostream & out) const {
    util::write(coord.x, out);
    util::write(coord.y, out);
    util::write(layer, out);
    util::write(blocked, out);

    sprite.serialize(out);
}

void MapObject::deserialize(std::istream & in) {
    util::read(coord.x, in);
    util::read(coord.y, in);
    util::read(layer, in);
    util::read(blocked, in);

    sprite.deserialize(in);
}

MapObject::~MapObject() {
}

MapObject::MapObject(RenderSystem & render) {
    sprite.init(render)
        .set_spritecoords(sf::IntRect{0, 0, 128, 128})
        .set_size(128, 128)
        .set_offset(TILEW/2 , TILEH/2 + CELLH/2);
}
            
void MapObject::set_coordinate(const sf::Vector2f & c) {
    coord = c;
    auto pos = util::to_pixel(coord);
    sprite.set_position(pos.x, pos.y);
}

void MapObject::set_blocked(bool b) {
    blocked = b;
}

void MapObject::move(const sf::Vector2f & offset) {
    set_coordinate(coord + offset);
}

void MapObject::set_layer(int z) {
    layer = z;
    //sprite.set_layer(z);
}

sf::Vector2f MapObject::coordinate() const {
    return coord;
}

int MapObject::get_layer() const {
    return layer;
}

bool MapObject::is_blocked() const {
    return blocked;
}

} // map

