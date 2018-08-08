#include <cassert>
#include "sprite.hpp"

namespace gfx {
inline void set_pos_verts(const sf::IntRect & rect, sf::Vertex * vs) {
    vs[0].position.x = rect.left;
    vs[0].position.y = rect.top;
    vs[1].position.x = rect.left + rect.width;
    vs[1].position.y = rect.top;
    vs[2].position.x = rect.left + rect.width;
    vs[2].position.y = rect.top + rect.height;
    vs[3].position.x = rect.left;
    vs[3].position.y = rect.top + rect.height;
}
inline void set_sprcrd(const sf::IntRect & rect, sf::Vertex * vs) {
    vs[0].texCoords.x = rect.left;
    vs[0].texCoords.y = rect.top;
    vs[1].texCoords.x = rect.left + rect.width;
    vs[1].texCoords.y = rect.top;
    vs[2].texCoords.x = rect.left + rect.width;
    vs[2].texCoords.y = rect.top + rect.height;
    vs[3].texCoords.x = rect.left;
    vs[3].texCoords.y = rect.top + rect.height;
}

namespace impl {
Primitive::Primitive(const id_t myid) : _id(myid) {
}

void Primitive::set_layer(int layer) {
    z = layer;
}

void Primitive::set_origin(const sf::Vector2i & p) {
    origin = p;
}

void Primitive::set_position(const sf::Vector2f & p) {
    coords.left = p.x + origin.x;
    coords.top = p.y + origin.y;
}

void Primitive::set_size(const sf::Vector2i & s) {
    coords.width = s.x;
    coords.height = s.y;
    spritecoords.width = coords.width;
    spritecoords.height = coords.height;
}

void Primitive::set_spritecoord(const sf::Vector2i & s) {
    spritecoords.left = s.x;
    spritecoords.top = s.y;
}

void Primitive::set_visible(bool b) {
    visible = b;
}

int Primitive::get_layer() const {
    return z;
}

sf::Vector2i Primitive::get_origin() const {
    return origin;
}

sf::Vector2f Primitive::get_position() const {
    return sf::Vector2f(coords.left, coords.top);
}

sf::IntRect Primitive::get_coords() const {
    return coords;
}

sf::IntRect Primitive::get_spritecoords() const {
    return spritecoords;
}

bool Primitive::is_visible() const {
    return visible;
}

bool Primitive::intersects(const sf::IntRect & rect) const {
    return coords.intersects(rect);
}

bool Primitive::operator==(const id_t others_id) {
    return _id == others_id;
}

bool Primitive::operator>(const Primitive & other) const {
    if (z == other.z) {
        if (coords.top == other.coords.top) {
            return coords.left >= other.coords.left;
        }
        else {
            return coords.top > other.coords.top;
        }
    }
    else {
        return z > other.z;
    }
}

bool Primitive::operator<(const Primitive & other) const {
    return !(*this > other);
}

id_t Primitive::id() const {
    return _id;
}

void Primitive::__draw(sf::Vertex * vs, std::size_t & idx) const {
    if (visible) {
        set_pos_verts(coords, vs);
        set_sprcrd(spritecoords, vs);
        idx += 4;
    }
}
} // impl

// Sprite

void Sprite::serialize(std::ostream & out) const {
    const auto & sp = get_primitive();

    //const sf::IntRect coords = sp.get_coords();
    const sf::IntRect spcoords = sp.get_spritecoords();
    const int layer = sp.get_layer();
    //const sf::Vector2i off = sp.get_origin();
    //const bool visible = sp.is_visible();

    //write(coords.left, out);
    //write(coords.top, out);
    //write(coords.width, out);
    //write(coords.height, out);

    write(spcoords.left, out);
    write(spcoords.top, out);

    write(layer, out);
    //write(off.x, out);
    //write(off.y, out);
    //write(visible, out);
}

void Sprite::deserialize(std::istream & in) {
    //sf::IntRect coords;
    sf::Vector2i spcoords;
    int layer;
    //sf::Vector2i off;
    //bool visible;

    //read(coords.left, in);
    //read(coords.top, in);
    //read(coords.width, in);
    //read(coords.height, in);

    read(spcoords.x, in);
    read(spcoords.y, in);

    read(layer, in);
    //read(off.x, in);
    //read(off.y, in);
    //read(visible, in);
    
    auto & sp = get_primitive();
    //sp.set_position(sf::Vector2f(coords.left, coords.top));
    //sp.set_size(sf::Vector2i{coords.width, coords.height});
    sp.set_spritecoord(spcoords);
    sp.set_layer(layer);
    //sp.set_origin(off);
    //sp.set_visible(visible);
}

const impl::Primitive & Sprite::get_primitive() const {
    assert(this->manager);
    return manager->get(id, "get_primitive");
}

impl::Primitive & Sprite::get_primitive() {
    assert(this->manager);
    if (id == 0) {
        id = manager->create();
    }
    return manager->get(id, "get_primitive");
}

Sprite::~Sprite() {
    if (manager) {
        manager->remove(id);
    }
}

Sprite::Sprite() {
}

Sprite::Sprite(Manager * m) {
    manager = m;
}

Sprite::Sprite(const Sprite & other) {
    assert(other.manager);
    this->operator=(other);
}

Sprite & Sprite::operator=(Sprite && other) {
    assert(other.manager);
    manager = other.manager;
    other.manager = nullptr;
    id = other.id;
    other.id = 0;
    return *this;
}

Sprite & Sprite::operator=(const Sprite & other) {
    assert(other.manager);
    manager = other.manager;
    manager->remove(id);
    id = manager->create();
    if (other.id) {
        auto & sp = get_primitive();
        auto & other_sp = manager->get(other.id, "operator=");

        sp.set_layer(other_sp.get_layer());
        auto coords = other_sp.get_coords();
        // must set position before origin
        sp.set_position(sf::Vector2f(coords.left, coords.top));
        sp.set_origin(other_sp.get_origin());
        sp.set_size(sf::Vector2i{coords.width, coords.height});
        auto spritecoords = other_sp.get_spritecoords();
        sp.set_spritecoord(sf::Vector2i{spritecoords.left, spritecoords.top});
        sp.set_visible(other_sp.is_visible());
    }

    return *this;
}

void Sprite::set_layer(int layer) {
    //assert(manager);
    auto & sp = get_primitive();
    sp.set_layer(layer);
}

void Sprite::set_origin(const sf::Vector2i & p) {
    //assert(manager);
    auto & sp = get_primitive();
    sp.set_origin(p);
}

void Sprite::set_position(const sf::Vector2f & p) {
    //assert(manager);
    auto & sp = get_primitive();
    sp.set_position(p);
}

void Sprite::set_size(const sf::Vector2i & s) {
    //assert(manager);
    auto & sp = get_primitive();
    sp.set_size(s);
}

void Sprite::set_spritecoord(const sf::Vector2i & s) {
    //assert(manager);
    auto & sp = get_primitive();
    sp.set_spritecoord(s);
}

void Sprite::set_visible(bool b) {
    auto & sp = get_primitive();
    sp.set_visible(b);
}

bool Sprite::operator==(const Sprite & other) const {
    const auto & spa = get_primitive();
    const auto & spb = other.get_primitive();
    return spa.get_layer() == spb.get_layer() 
        && spa.get_coords() == spb.get_coords()
        && spa.get_spritecoords() == spb.get_spritecoords();
}

// Manager

id_t Manager::create(const std::string & why) {
    sprites.emplace_back(++next_id);
    if (why != "") {
        std::cout << "Created sprite " << next_id 
            << ", why=" << why << std::endl;
    }
    return sprites.back().id();
}

void Manager::remove(const id_t id) {
    auto cmp = [id](const impl::Primitive & s){ return s.id() == id; };
    auto itr = std::find_if(sprites.begin(), sprites.end(), cmp);
    auto last = sprites.rbegin();
    if (itr != sprites.end()) {
        *itr = *last;
        sprites.pop_back();
    }
}

impl::Primitive & Manager::get(const id_t id, const std::string & why) {
    auto cmp = [id](const impl::Primitive & s){ return s.id() == id; };
    auto itr = std::find_if(sprites.begin(), sprites.end(), cmp);
    if (itr != sprites.end()) {
        return *itr;
    }
    else {
        auto strid = std::to_string(id) + ", why = " + why;
        throw std::logic_error{"get_sprite on nonexistent id " + strid};
    }
}

void Manager::draw(sf::RenderWindow & window) {
    auto view = window.getView();
    auto center = sf::Vector2i(view.getCenter());
    auto size = sf::Vector2i(view.getSize());
    auto pos = sf::Vector2i(center - size / 2);
    sf::IntRect screen{pos, size};

    auto cmp = [this](const impl::Primitive * lhs, const impl::Primitive * rhs){
        return *lhs < *rhs;
    };
    std::vector<impl::Primitive *> visible_sprites;
    for (auto & sprite : sprites) {
        if (sprite.intersects(screen)) {
            visible_sprites.push_back(&sprite);
        }
    }
    std::sort(visible_sprites.begin(), visible_sprites.end(), cmp);

    static std::vector<sf::Vertex> vertices;
    if (vertices.size() < 4 * visible_sprites.size()) {
        vertices.resize(4 * visible_sprites.size());
    }
    std::size_t idx = 0;
    for (auto sprite : visible_sprites) {
        sprite->__draw(&vertices[idx], idx);
    }

    window.draw(&vertices[0], idx, sf::Quads, &texture);
}
} // gfx