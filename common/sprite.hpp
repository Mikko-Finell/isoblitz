#ifndef sprite_hpp
#define sprite_hpp

#include "util.hpp"
#include <SFML/Graphics.hpp>

class SpriteImpl {
    sf::FloatRect screencoords;
    sf::IntRect texcoords;

    // the distance from the visual center of the gameobject
    // to the actual corner of the sprite
    sf::Vector2i offset;

    int layer = config::default_sprite_layer;
    bool visible = true;

public:
    void draw(sf::Vertex * vs) const;

    SpriteImpl & set_texcoords(const sf::IntRect & coords);
    SpriteImpl & set_screencoords(const sf::FloatRect & coords);
    SpriteImpl & set_position(float x, float y);
    SpriteImpl & set_position(const sf::Vector2f & v);
    SpriteImpl & set_size(int w, int h);
    SpriteImpl & set_offset(int x, int y);
    SpriteImpl & set_layer(int z);

    // used for isometric sorting
    bool operator>(const SpriteImpl & other) const;
    inline bool operator<(const SpriteImpl & other) const {
        return !(operator>(other));
    }

    int get_layer() const;
    const sf::FloatRect & get_screencoords() const;
    const sf::IntRect & get_texcoords() const;

    // position is the actual upper-left corner of the sprite
    Position get_position() const;

    // origin is the visual center of the gameobject, for example
    // the feet of a soldier
    Position get_origin() const;

    std::string info() const;
};

class SpriteManager;
class SpriteFactory;
class RenderSystem;
class Sprite {
    friend SpriteFactory;
    SpriteImpl * impl = nullptr;
    RenderSystem * renders = nullptr;
    SpriteManager * spritem = nullptr;
    SpriteFactory * spritef = nullptr;

public:
    ~Sprite();
    Sprite();
    Sprite(RenderSystem * rs, SpriteManager * sm, SpriteFactory * sf, SpriteImpl * imp);
    Sprite(const Sprite & other);
    Sprite & operator=(const Sprite & other);
    Sprite & operator=(Sprite && other);

    void clear();

    Sprite & set_texcoords(const sf::IntRect & coords);
    Sprite & set_screencoords(const sf::FloatRect & coords);
    Sprite & set_position(float x, float y);
    Sprite & set_position(const sf::Vector2f & v);
    Sprite & set_size(int w, int h);
    Sprite & set_offset(int x, int y);
    Sprite & set_layer(int z);

    int get_layer() const;
    const sf::FloatRect & get_screencoords() const;
    const sf::IntRect & get_texcoords() const;

    // position is the actual upper-left corner of the sprite
    Position get_position() const;

    // origin is the visual center of the gameobject, for example
    // the feet of a soldier
    Position get_origin() const;

    std::string info() const;
};

#endif
