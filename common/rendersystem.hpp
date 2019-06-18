#ifndef rendersystem_hpp
#define rendersystem_hpp

#include "sprite.hpp"
#include <SFML/Graphics.hpp>
#include <unordered_set>

/**
 * RenderSystem
 * Base class for rendering gameobjects.
 */
class RenderSystem {
protected:
    std::unordered_set<SpriteImpl *> sprites;
    sf::Texture & texture;
    std::vector<sf::Vertex> vs;

public:
    virtual ~RenderSystem() {}
    RenderSystem(sf::Texture & tex);
    virtual void add(SpriteImpl * sprite, const std::string & caller);
    inline void add(SpriteImpl & sprite, const std::string & caller) {
        add(&sprite, caller);
    }

    // removes the sprite from set of sprites, preventing it from being drawn
    virtual void remove(SpriteImpl * sprite);

    virtual void draw(sf::RenderWindow & window) = 0;
};

/**
 * WorldRender
 * RenderSystem that knows about the isometric tile 
 * layering used in Bullet.
 */
class WorldRender : public RenderSystem {
    std::vector<SpriteImpl *> visible_sprites;

public:
    using RenderSystem::RenderSystem;
    void remove(SpriteImpl * sprite) override;
    inline void remove(SpriteImpl & sprite) {
        remove(&sprite);
    }
    void draw(sf::RenderWindow & window) override;
};

/**
 * UIRender
 * RenderSystem that uses simple layering appropriate 
 * for UI.
 */
class UIRender : public RenderSystem {
    std::vector<SpriteImpl *> sorted_sprites;
    bool sorted = false;

public:
    using RenderSystem::RenderSystem;
    void add(SpriteImpl * sprite, const std::string & caller) override;
    void remove(SpriteImpl * sprite) override;
    inline void remove(SpriteImpl & sprite) {
        remove(&sprite);
    }
    void draw(sf::RenderWindow & window) override;
};

#endif
