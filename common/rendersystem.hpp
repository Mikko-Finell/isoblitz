#ifndef rendersystem_hpp
#define rendersystem_hpp

#include <SFML/Graphics.hpp>
#include <unordered_set>
#include <vector>

struct SpriteData;

class RenderSystem {
protected:
    sf::Texture & texture;

public:
    virtual ~RenderSystem() {}
    RenderSystem(sf::Texture & tex) : texture(tex) {}
    virtual void add(SpriteData & data) = 0;
    virtual void remove(SpriteData & data) = 0;
    virtual void draw(sf::RenderWindow & window) = 0;
};

class WorldRender : public RenderSystem {
    std::unordered_set<SpriteData *> spritedata;

public:
    using RenderSystem::RenderSystem;
    void add(SpriteData & data) override;
    void remove(SpriteData & data) override;
    void draw(sf::RenderWindow & window) override;

};

class UIRender : public RenderSystem {
    std::unordered_set<SpriteData *> keys;
    std::vector<SpriteData *> spritedata;

public:
    using RenderSystem::RenderSystem;
    void add(SpriteData & data) override;
    void remove(SpriteData & data) override;
    void draw(sf::RenderWindow & window) override;
};

#endif
