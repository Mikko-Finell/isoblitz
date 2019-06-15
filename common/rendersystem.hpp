#ifndef rendersystem_hpp
#define rendersystem_hpp

#include "system.hpp"
#include <SFML/Graphics.hpp>
#include <unordered_set>

class Sprite;

/**
 * RenderSystem
 * Base class for rendering gameobjects.
 */
class RenderSystem { //: public System {
protected:
    std::unordered_set<Sprite *> sprites;
    sf::Texture & texture;

public:
    virtual ~RenderSystem() {}
    RenderSystem(sf::Texture & tex);
    //virtual void remove(GameObject * go) override;
    virtual bool add(Sprite * sprite, const std::string & caller);
    inline bool add(Sprite & sprite, const std::string & caller) {
        return add(&sprite, caller);
    }

    // removes the sprite from set of sprites but doesn't
    // decouple gameobj from system
    virtual void unlist(Sprite * sprite);

    virtual void draw(sf::RenderWindow & window) = 0;
};

/**
 * WorldRender
 * RenderSystem that knows about the isometric tile 
 * layering used in Bullet.
 */
class WorldRender : public RenderSystem {
public:
    using RenderSystem::RenderSystem;
    void remove(Sprite & sprite);
    void draw(sf::RenderWindow & window) override;
};

/**
 * UIRender
 * RenderSystem that uses simple layering appropriate 
 * for UI.
 */
class UIRender : public RenderSystem {
    std::vector<Sprite *> sorted_sprites;
    bool sorted = false;

public:
    using RenderSystem::RenderSystem;
    //void remove(GameObject * go) override;
    bool add(Sprite * sprite, const std::string & caller) override;
    void remove(Sprite & sprite);
    void unlist(Sprite * sprite) override;
    void draw(sf::RenderWindow & window) override;
};

#endif
