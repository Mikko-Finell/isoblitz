#ifndef sprite_hpp
#define sprite_hpp

#include "rendersystem.hpp"
#include <SFML/Graphics.hpp>

struct SpriteData {
    sf::IntRect screencoords;
    sf::IntRect spritecoords;
    sf::Vector2i offset;
    int layer = 0;
};

class Sprite {
    RenderSystem * render = nullptr;
    bool visible = false;

    SpriteData data;

public:
    ~Sprite();
    Sprite();
    Sprite(RenderSystem & rs);
    Sprite(const Sprite & other);
    Sprite & operator=(const Sprite & other);
    Sprite & operator=(const SpriteData & data);
    Sprite & init(RenderSystem * rs);
    Sprite & init(RenderSystem & rs);
    Sprite & show();
    Sprite & hide();
    Sprite & set_position(int x, int y);
    Sprite & set_position(const sf::Vector2f & v);
    Sprite & set_size(int w, int h);
    Sprite & set_screencoords(const sf::IntRect & coords);
    Sprite & set_spritecoords(const sf::IntRect & coords);
    Sprite & set_spritecoord(const sf::Vector2i & coords);
    Sprite & set_spritecoord(int x, int y);
    Sprite & set_data(const SpriteData & d);
    Sprite & set_offset(int x, int y);
    Sprite & set_layer(int z);
    SpriteData get_spritedata() const;

    void serialize(std::ostream & out) const;
    void deserialize(std::istream & in);

    bool operator==(const Sprite & other) const;

};

#endif
