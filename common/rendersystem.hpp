#ifndef __rendersystem_hpp__
#define __rendersystem_hpp__

#include <SFML/Graphics.hpp>
#include <unordered_set>

struct SpriteData;

class RenderSystem {
    std::unordered_set<SpriteData *> spritedata;
    sf::Texture texture;

public:
    inline void load_texture(const std::string & name) {
        texture.loadFromFile(name);
    }
    void add(SpriteData & data);
    void remove(SpriteData & data);
    void draw(sf::RenderWindow & window);
};

#endif
