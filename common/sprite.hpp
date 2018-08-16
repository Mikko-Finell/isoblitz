#ifndef __SPRITE__
#define __SPRITE__

#include "rendersystem.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include <unordered_set>

struct SpriteData {
    sf::IntRect screencoords;
    sf::IntRect spritecoords;
    sf::Vector2i offset;
    int layer = 0;
};

class Sprite {
public:
    SpriteData data;
    RenderSystem * render = nullptr;
    bool visible = false;
    
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
    Sprite & set_size(int w, int h);
    Sprite & set_screencoords(const sf::IntRect & coords);
    Sprite & set_spritecoords(const sf::IntRect & coords);
    Sprite & set_spritecoord(const sf::Vector2i & coords);
    Sprite & set_spritecoord(int x, int y);
    Sprite & set_data(const SpriteData & d);
    Sprite & set_offset(int x, int y);
    Sprite & set_layer(int z);

    void serialize(std::ostream & out) const;
    void deserialize(std::istream & in);

    bool operator==(const Sprite & other) const;
};

#endif
