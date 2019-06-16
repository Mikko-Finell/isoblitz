#ifndef engine_hpp
#define engine_hpp

#include "camera.hpp"
#include "input.hpp"
#include "rendersystem.hpp"
#include "spritemanager.hpp"
#include "spritefactory.hpp"
#include "animationmanager.hpp"
#include "animationfactory.hpp"
#include "entity.hpp"
#include "entityfactory.hpp"
#include "entitymanager.hpp"
#include "tilefactory.hpp"
#include "tilemanager.hpp"

/** Engine
 * Manager for a game state. There can only be one.
 *
 * TODO hard nicetohave
 * create (subclass?) ability to run game logic
 * without an active sf::RenderWindow instance.
 */
class Engine {
    bool pause = false;

    void poll_events();
    void draw(const sf::Color & bgcolor = sf::Color::White);
    void update();

public:
    sf::RenderWindow    window;
    sf::Texture         texture;
    Camera              camera;
    input::Manager      inputm;
    input::Context      globctx; 
    WorldRender         wrender;
    UIRender            uirender;
    SpriteManager       spritem;
    SpriteFactory       spritef;
    AnimationSystem     anims;
    AnimationManager    animm;
    AnimationFactory    animf;
    EntityFactory       entityf;
    EntitySystem        entitys;
    EntityManager       entitym;
    TileFactory         tilef;
    TileManager         tilem;

    ~Engine() {}
    Engine();
    void init();
    void run();
    void reset();

    virtual bool is_running() const;
};

#endif
