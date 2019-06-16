#ifndef sprite_hpp
#define sprite_hpp

#include "rendersystem.hpp"
#include "util.hpp"
#include <SFML/Graphics.hpp>

/**
 * Sprite
 * Used for all sprites in Bullet.
 */
class Sprite {
    sf::FloatRect screencoords;
    sf::IntRect spritecoords;

    // the distance from the visual center of the gameobject
    // to the actual corner of the sprite
    sf::Vector2i offset;

    int layer = config::default_sprite_layer;
    bool visible = true;

public:
    RenderSystem * renderer = nullptr;

    ~Sprite();
    Sprite();

    Sprite(const Sprite & other);
    Sprite & operator=(const Sprite & other);

    void draw(sf::Vertex * vs) const;

    Sprite & set_spritecoords(const sf::IntRect & coords);
    Sprite & set_screencoords(const sf::FloatRect & coords);
    Sprite & set_position(float x, float y);
    Sprite & set_position(const sf::Vector2f & v);
    Sprite & set_size(int w, int h);
    Sprite & set_offset(int x, int y);
    Sprite & set_layer(int z);

    // used for isometric sorting
    bool operator>(const Sprite & other) const;
    inline bool operator<(const Sprite & other) const {
        return !(operator>(other));
    }

    int get_layer() const;
    const sf::FloatRect & get_screencoords() const;
    const sf::IntRect & get_spritecoords() const;

    // position is the actual upper-left corner of the sprite
    Position get_position() const;

    // origin is the visual center of the gameobject, for example
    // the feet of a soldier
    Position get_origin() const;

    std::string info() const;
};

#endif
