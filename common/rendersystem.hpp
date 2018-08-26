#ifndef rendersystem_hpp
#define rendersystem_hpp

#include "system.hpp"
#include <SFML/Graphics.hpp>
#include <unordered_set>

class Sprite;

class RenderSystem : public System {
protected:
    std::unordered_set<Sprite *> sprites;
    sf::Texture & texture;

public:
    virtual ~RenderSystem() {}
    RenderSystem(sf::Texture & tex);
    virtual void remove(GameObject * go) override;
    virtual bool add(Sprite * sprite);
    inline bool add(Sprite & sprite) {
        return add(&sprite);
    }
    virtual void unlist(Sprite * sprite);
    virtual void draw(sf::RenderWindow & window) = 0;
};

class WorldRender : public RenderSystem {
public:
    using RenderSystem::RenderSystem;
    void draw(sf::RenderWindow & window) override;
};

class UIRender : public RenderSystem {
    std::vector<Sprite *> sorted_sprites;
    bool sorted = false;

public:
    using RenderSystem::RenderSystem;
    void remove(GameObject * go) override;
    bool add(Sprite * sprite) override;
    void unlist(Sprite * sprite) override;
    void draw(sf::RenderWindow & window) override;
};

#endif
