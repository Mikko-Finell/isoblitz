#include "sprite.hpp"
#include "util.hpp"
#include <iostream>
#include <cassert>

namespace {
// set sprite position in the world
inline void vert_set_pos(sf::Vertex * vs, const sf::FloatRect & rect) {
    vs[0].position.x = rect.left;
    vs[0].position.y = rect.top;
    vs[1].position.x = rect.left + rect.width;
    vs[1].position.y = rect.top;
    vs[2].position.x = rect.left + rect.width;
    vs[2].position.y = rect.top + rect.height;
    vs[3].position.x = rect.left;
    vs[3].position.y = rect.top + rect.height;
}

// set sprites texture coords
inline void vert_set_crd(sf::Vertex * vs, const sf::IntRect & rect) {
    vs[0].texCoords.x = rect.left;
    vs[0].texCoords.y = rect.top;
    vs[1].texCoords.x = rect.left + rect.width;
    vs[1].texCoords.y = rect.top;
    vs[2].texCoords.x = rect.left + rect.width;
    vs[2].texCoords.y = rect.top + rect.height;
    vs[3].texCoords.x = rect.left;
    vs[3].texCoords.y = rect.top + rect.height;
}
}

Sprite::~Sprite() {
    unreg();
}

Sprite::Sprite() {
}

Sprite::Sprite(RenderSystem & rs) {
    rs.add(this);
}

Sprite::Sprite(const Sprite & other) {
    operator=(other);
}

Sprite & Sprite::operator=(const Sprite & other) {
    screencoords = other.screencoords;
    spritecoords = other.spritecoords;
    offset = other.offset;
    layer = other.layer;
    visible = other.visible;

    if (other.system) {
        system = other.system;
    }
    if (visible) {
        show();
    }
    return *this;
}

void Sprite::draw(sf::Vertex * vs) const {
    vert_set_pos(vs, screencoords);
    vert_set_crd(vs, spritecoords);
}

Sprite & Sprite::show() {
    if (auto rs = dynamic_cast<RenderSystem *>(system); rs != nullptr) {
        rs->add(this);
    }
    return *this;
}

Sprite & Sprite::hide() {
    if (auto rs = dynamic_cast<RenderSystem *>(system); rs != nullptr) {
        rs->unlist(this);
    }
    return *this;
}

Sprite & Sprite::set_spritecoords(const sf::IntRect & coords) {
    spritecoords = coords;
    return *this;
}

Sprite & Sprite::set_screencoords(const sf::FloatRect & coords) {
    screencoords = coords;
    return *this;
}

Sprite & Sprite::set_position(float x, float y) {
    screencoords.left = x - offset.x;
    screencoords.top = y - offset.y;
    return *this;
}

Sprite & Sprite::set_position(const sf::Vector2f & v) {
    return set_position(v.x, v.y);
}

Sprite & Sprite::set_size(int w, int h) {
    screencoords.width = w;
    screencoords.height = h;
    return *this;
}

Sprite & Sprite::set_offset(int x, int y) {
    offset.x = x;
    offset.y = y;
    return *this;
}

Sprite & Sprite::set_layer(int z) {
    layer = z;
    return *this;
}

// isometric sort relation
bool Sprite::operator>(const Sprite & other) const {
    if (layer == other.layer) {
        if (screencoords.top == other.screencoords.top) {
            return screencoords.left >= other.screencoords.left;
        }
        else {
            return screencoords.top > other.screencoords.top;
        }
    }
    else {
        return layer > other.layer;
    }
}

// TODO possibly remove these methods
void Sprite::serialize(std::ostream & out) const {
    util::write(offset.x, out);
    util::write(offset.y, out);

    util::write(screencoords.left, out);
    util::write(screencoords.top, out);
    util::write(screencoords.width, out);
    util::write(screencoords.height, out);

    util::write(spritecoords.left, out);
    util::write(spritecoords.top, out);
    util::write(spritecoords.width, out);
    util::write(spritecoords.height, out);

    util::write(layer, out);
    util::write(visible, out);
}

void Sprite::deserialize(std::istream & in) {
    util::read(offset.x, in);
    util::read(offset.y, in);

    util::read(screencoords.left, in);
    util::read(screencoords.top, in);
    util::read(screencoords.width, in);
    util::read(screencoords.height, in);

    util::read(spritecoords.left, in);
    util::read(spritecoords.top, in);
    util::read(spritecoords.width, in);
    util::read(spritecoords.height, in);

    util::read(layer, in);
    util::read(visible, in);
}
