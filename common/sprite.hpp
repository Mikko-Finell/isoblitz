#ifndef __SPRITE__
#define __SPRITE__

#include "serializable.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

using VertexArray = std::vector<sf::Vertex>;

namespace gfx {

using id_t = std::size_t;

namespace impl {
class Primitive {
    id_t _id = 0;

    sf::Vector2i origin{0, 0};
    sf::IntRect coords{0, 0, 128, 128};
    sf::IntRect spritecoords{256, 0, 128, 128};
    int z = 0;
    bool visible = true;

public:
    virtual ~Primitive() {}
    Primitive(const id_t myid);

    void set_layer(int layer);
    void set_origin(const sf::Vector2i & p);
    void set_position(const sf::Vector2f & p);
    void set_size(const sf::Vector2i & s);
    void set_spritecoord(const sf::Vector2i & s);
    void set_visible(bool b);

    int get_layer() const;
    sf::Vector2i get_origin() const;
    sf::Vector2f get_position() const;
    sf::IntRect get_coords() const;
    sf::IntRect get_spritecoords() const;

    bool is_visible() const;
    bool intersects(const sf::IntRect & rect) const;
    bool operator==(const id_t others_id);
    bool operator>(const Primitive & other) const;
    bool operator<(const Primitive & other) const;
    id_t id() const;
    void __draw(sf::Vertex * vs, std::size_t & idx) const;
};
} // impl

class SpriteManager;

class Sprite : public Serializable {
    id_t id = 0;
    SpriteManager * manager = nullptr;

    void serialize(std::ostream & out) const override;
    void deserialize(std::istream & in) override;
    impl::Primitive & get_primitive();
    const impl::Primitive & get_primitive() const;

public:
    virtual ~Sprite();
    Sprite();
    Sprite(SpriteManager * m);
    Sprite(const Sprite & other);
    Sprite & operator=(Sprite && other);
    Sprite & operator=(const Sprite & other);

    //sf::Vector2f get_position() const;

    void set_layer(int layer);
    void set_origin(int x, int y);
    void set_position(const sf::Vector2f & p);
    void set_size(int w, int h);
    void set_spritecoord(const sf::Vector2i & s);
    void set_spritecoord(const sf::IntRect & r);
    void set_visible(bool b);

    bool operator==(const Sprite & other) const;
};

class SpriteManager {
    std::vector<impl::Primitive> sprites;
    id_t next_id = 0;

public:
    sf::Texture texture;

    id_t create();
    void remove(const id_t id);
    impl::Primitive & get(const id_t id);
    void draw(sf::RenderWindow & window);
};

} // gfx

#endif
