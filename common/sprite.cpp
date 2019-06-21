#include "spritemanager.hpp"
#include "spritefactory.hpp"
#include "rendersystem.hpp"
#include "sprite.hpp"
#include "util.hpp"
#include <iostream>
#include <sstream>
#include <cassert>

namespace {
// set impl position in the world
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

// set impls texture coords
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

void SpriteImpl::draw(sf::Vertex * vs) const {
    vert_set_pos(vs, screencoords);
    vert_set_crd(vs, texcoords);
}

SpriteImpl & SpriteImpl::set_texcoords(const sf::IntRect & coords) {
    texcoords = coords;
    return *this;
}

SpriteImpl & SpriteImpl::set_screencoords(const sf::FloatRect & coords) {
    screencoords = coords;
    return *this;
}

SpriteImpl & SpriteImpl::set_position(float x, float y) {
    screencoords.left = x - offset.x;
    screencoords.top = y - offset.y;
    return *this;
}

SpriteImpl & SpriteImpl::set_position(const sf::Vector2f & v) {
    return set_position(v.x, v.y);
}

SpriteImpl & SpriteImpl::set_size(int w, int h) {
    int ox = offset.x, oy = offset.y;
    if (screencoords.width != 0) {
        ox = ox * w / screencoords.width;
    }
    if (screencoords.height != 0) {
        oy = oy * h / screencoords.height;
    }
    set_offset(ox, oy);

    screencoords.width = w;
    screencoords.height = h;
    return *this;

}

SpriteImpl & SpriteImpl::set_offset(int x, int y) {
    offset.x = x;
    offset.y = y;
    return *this;
}

SpriteImpl & SpriteImpl::set_layer(int z) {
    layer = z;
    return *this;
}

// isometric sort relation
bool SpriteImpl::operator<(const SpriteImpl & other) const {
    if (layer == other.layer) {
        const auto mypos = get_visual_center();
        const auto yourpos = other.get_visual_center();
        if (mypos.y == yourpos.y) {
            return mypos.x < yourpos.x;
        }
        else {
            return mypos.y < yourpos.y;
        }
    }
    else {
        return layer < other.layer;
    }
}

int SpriteImpl::get_layer() const {
    return layer;
}

const sf::FloatRect & SpriteImpl::get_screencoords() const {
    return screencoords;
}

const sf::IntRect & SpriteImpl::get_texcoords() const {
    return texcoords;
}

// position is the actual upper-left corner of the impl
Position SpriteImpl::get_position() const {
    return Position{screencoords.left, screencoords.top};
}

// visual center is the visual center of the gameobject, for example
// the feet of a soldier
Position SpriteImpl::get_visual_center() const {
    return get_position() + offset;
}

std::string SpriteImpl::info() const {
    std::stringstream ss; ss << "SpriteImpl:\n"
        << "\tScreencoords{" << util::rect_to_str(screencoords) << "}\n"
        << "\tTexcoords{" << util::rect_to_str(texcoords) << "}\n"
        << "\tOffset{" << offset.x << ", " << offset.y << "}\n"
        << "\tLayer = " << layer << "\n";
        //<< std::boolalpha << "\tVisible = " << visible << std::endl;
    return ss.str();
}
////////////////////////////////////////////////////////////////// SPRITE

Sprite::~Sprite() {
    clear();
}

Sprite::Sprite() {
}

Sprite::Sprite(RenderSystem * rs, SpriteManager * sm, SpriteFactory * sf, SpriteImpl * imp) 
    : impl(imp), renders(rs), spritef(sf), spritem(sm)
{
}

Sprite::Sprite(const Sprite & other) {
    operator=(other);
}

Sprite & Sprite::operator=(const Sprite & other) {
    // Sanity checks + cleanup
    if (impl != nullptr || renders != nullptr || spritef != nullptr || spritem != nullptr) {
        assert(renders != nullptr);
        assert(spritem != nullptr);
        assert(spritef != nullptr);
        assert(impl != nullptr);
    }
    if (impl == nullptr || renders == nullptr || spritef == nullptr || spritem == nullptr) {
        assert(renders == nullptr);
        assert(spritem == nullptr);
        assert(spritef == nullptr);
        assert(impl == nullptr);
    }

    clear();

    spritem = other.spritem;
    spritef = other.spritef;
    renders = other.renders;

    if (spritef != nullptr && renders != nullptr) {
        impl = spritef->copy(*renders, other.impl);
    }
    show(other.visible);

    return *this;
}

Sprite & Sprite::operator=(Sprite && other) {
    operator=(other);
    other.clear();
    return *this;
}

void Sprite::show(bool on) {
    visible = on;
    if (impl != nullptr) {
        assert(renders != nullptr);

        if (visible) {
            try {
                renders->add(impl, "Sprite::show");
            }
            catch (std::logic_error) {
            }
        }
        else {
            hide();
        }
    }
}

void Sprite::hide(bool on) {
    visible = !on;
    if (impl != nullptr) {
        assert(renders != nullptr);

        if (!visible) {
            try {
                renders->remove(impl);
            }
            catch (std::logic_error) {
            }
        }
        else {
            show();
        }
    }
}

void Sprite::clear() {
    if (renders != nullptr and visible == true) {
        renders->remove(impl);
    }
    if (spritem != nullptr) {
        spritem->destroy(impl);
    }
    renders = nullptr;
    spritem = nullptr;
    spritef = nullptr;
    impl = nullptr;
}

Sprite & Sprite::set_texcoords(const sf::IntRect & coords) {
    assert(impl != nullptr);
    impl->set_texcoords(coords);
    return *this;
}

Sprite & Sprite::set_screencoords(const sf::FloatRect & coords) {
    assert(impl != nullptr);
    impl->set_screencoords(coords);
    return *this;
}

Sprite & Sprite::set_position(float x, float y) {
    assert(impl != nullptr);
    impl->set_position(x, y);
    return *this;
}

Sprite & Sprite::set_position(const sf::Vector2f & v) {
    assert(impl != nullptr);
    return set_position(v.x, v.y);
}

Sprite & Sprite::set_size(int w, int h) {
    assert(impl != nullptr);
    impl->set_size(w, h);
    return *this;
}

Sprite & Sprite::set_offset(int x, int y) {
    assert(impl != nullptr);
    impl->set_offset(x, y);
    return *this;
}

Sprite & Sprite::set_layer(int z) {
    assert(impl != nullptr);
    impl->set_layer(z);
    return *this;
}

int Sprite::get_layer() const {
    assert(impl != nullptr);
    return impl->get_layer();
}

const sf::FloatRect & Sprite::get_screencoords() const {
    assert(impl != nullptr);
    return impl->get_screencoords();
}

const sf::IntRect & Sprite::get_texcoords() const {
    assert(impl != nullptr);
    return impl->get_texcoords();
}

// position is the actual upper-left corner of the impl
Position Sprite::get_position() const {
    assert(impl != nullptr);
    return impl->get_position();
}

// origin is the visual center of the gameobject, for example
// the feet of a soldier
Position Sprite::get_visual_center() const {
    assert(impl != nullptr);
    return impl->get_visual_center();
}

std::string Sprite::info() const {
    std::stringstream ss;
    ss << "Sprite:\n\timpl=" << impl
        << "\n\trenders=" << renders 
        << "\n\tspritem=" << spritem
        << "\n\tspritef=" << spritef << std::endl;
    if (impl != nullptr) {
        ss << impl->info();
    }
    else {
        ss << "SpriteImpl is null.";
    }
    return ss.str();
}

