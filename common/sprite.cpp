#include "sprite.hpp"
#include "util.hpp"
#include <iostream>
#include <cassert>

Sprite::Sprite() {
}

Sprite::Sprite(RenderSystem & rs) {
    init(rs);
}

Sprite::Sprite(const Sprite & other) {
    this->data = other.data;
    init(other.render);
}

Sprite & Sprite::operator=(const Sprite & other) {
    this->data = other.data;
    init(other.render);
    return *this;
}

Sprite & Sprite::operator=(const SpriteData & data) {
    this->data = data;
    return *this;
}

Sprite::~Sprite() {
    if (render) {
        render->remove(data);
    }
}

Sprite & Sprite::init(RenderSystem * rs) {
    if (rs) {
        init(*rs);
    }
    return *this;
}

Sprite & Sprite::init(RenderSystem & rs) {
    render = &rs;
    show();
    return *this;
}

Sprite & Sprite::show() {
    assert(render);
    render->add(data);
    visible = true;
    return *this;
}

Sprite & Sprite::hide() {
    assert(render);
    render->remove(data);
    visible = false;
    return *this;
}

Sprite & Sprite::set_position(int x, int y) {
    data.screencoords.left = x - data.offset.x;
    data.screencoords.top = y - data.offset.y;
    return *this;
}

Sprite & Sprite::set_size(int w, int h) {
    data.screencoords.width = w;
    data.screencoords.height = h;
    return *this;
}

Sprite & Sprite::set_screencoords(const sf::IntRect & coords) {
    data.screencoords = coords;
    return *this;
}

Sprite & Sprite::set_spritecoords(const sf::IntRect & coords) {
    data.spritecoords = coords;
    return *this;
}

Sprite & Sprite::set_spritecoord(const sf::Vector2i & coords) {
    return set_spritecoord(coords.x, coords.y);
}

Sprite & Sprite::set_spritecoord(int x, int y) {
    data.spritecoords.left = x;
    data.spritecoords.top = y;
    return *this;
}

Sprite & Sprite::set_data(const SpriteData & d) {
    data = d;
    return *this;
}

Sprite & Sprite::set_offset(int x, int y) {
    data.offset = sf::Vector2i{x, y};
    return *this;
}

void Sprite::serialize(std::ostream & out) const {
    util::write(data.screencoords.left, out);
    util::write(data.screencoords.top, out);
    util::write(data.screencoords.width, out);
    util::write(data.screencoords.height, out);
    util::write(data.spritecoords.left, out);
    util::write(data.spritecoords.top, out);
    util::write(data.layer, out);
    util::write(data.offset.x, out);
    util::write(data.offset.y, out);
    util::write(visible, out);
}

void Sprite::deserialize(std::istream & in) {
    util::read(data.screencoords.left, in);
    util::read(data.screencoords.top, in);
    util::read(data.screencoords.width, in);
    util::read(data.screencoords.height, in);
    util::read(data.spritecoords.left, in);
    util::read(data.spritecoords.top, in);
    util::read(data.layer, in);
    util::read(data.offset.x, in);
    util::read(data.offset.y, in);
    util::read(visible, in);
}

Sprite & Sprite::set_layer(int z) {
    data.layer = z;
    return *this;
}

bool Sprite::operator==(const Sprite & other) const {
    return data.spritecoords == other.data.spritecoords &&
        data.screencoords == other.data.screencoords &&
        data.layer == other.data.layer;
}

bool operator>(const SpriteData & a, const SpriteData & b) {
    if (a.layer == b.layer) {
        if (a.screencoords.top == b.screencoords.top) {
            return a.screencoords.left >= b.screencoords.left;
        }
        else {
            return a.screencoords.top > b.screencoords.top;
        }
    }
    else {
        return a.layer > b.layer;
    }
}

bool operator<(const SpriteData & a, const SpriteData & b) {
    return !(a > b);
}
