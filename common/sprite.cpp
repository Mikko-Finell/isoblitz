#include "sprite.hpp"
#include "util.hpp"
#include <iostream>
#include <sstream>
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
    if (renderer != nullptr) {
        renderer->remove(this);
    }
}

Sprite::Sprite() {
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
    return *this;
}

void Sprite::draw(sf::Vertex * vs) const {
    vert_set_pos(vs, screencoords);
    vert_set_crd(vs, spritecoords);
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

int Sprite::get_layer() const {
    return layer;
}

const sf::FloatRect & Sprite::get_screencoords() const {
    return screencoords;
}

const sf::IntRect & Sprite::get_spritecoords() const {
    return spritecoords;
}

// position is the actual upper-left corner of the sprite
Position Sprite::get_position() const {
    return Position{screencoords.left, screencoords.top};
}

// origin is the visual center of the gameobject, for example
// the feet of a soldier
Position Sprite::get_origin() const {
    return get_position() + offset;
}

std::string Sprite::info() const {
    std::stringstream ss; ss << "Sprite:\n"
        << "\tScreencoords{" << util::rect_to_str(screencoords) << "}\n"
        << "\tSpritecoords{" << util::rect_to_str(spritecoords) << "}\n"
        << "\tOffset{" << util::vec_to_str(offset) << "}\n"
        << "\tLayer = " << layer << "\n"
        << std::boolalpha << "\tVisible = " << visible << std::endl;
    return ss.str();
}
