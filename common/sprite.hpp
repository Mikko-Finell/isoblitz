#ifndef __SPRITE__
#define __SPRITE__

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

class SpriteManager {
    std::unordered_set<SpriteData *> spritedata;
    using key_t = std::string;
    std::unordered_map<key_t, std::unordered_map<key_t, SpriteData>> entitymap;
    sf::Texture texture;

public:
    SpriteManager();
    void add(SpriteData & data);
    SpriteData get(const std::string & entity, const std::string & sprite);
    void remove(SpriteData & data);
    void draw(sf::RenderWindow & window);
    inline void load_texture(const std::string & name) {
        texture.loadFromFile(name);
    }
};

class Sprite {
public:
    SpriteData data;
    SpriteManager * spritem = nullptr;
    bool visible = false;
    
public:
    Sprite();
    Sprite(SpriteManager & sm);
    Sprite(const Sprite & other);
    Sprite & operator=(const Sprite & other);
    Sprite & operator=(const SpriteData & data);
    ~Sprite();
    Sprite & init(SpriteManager * sm);
    Sprite & init(SpriteManager & sm);
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
