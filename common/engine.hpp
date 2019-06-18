#ifndef engine_hpp
#define engine_hpp

#include "camera.hpp"
#include "input.hpp"
#include "rendersystem.hpp"
#include "spritemanager.hpp"
#include "spritefactory.hpp"
#include "animationfactory.hpp"
#include "entity.hpp"
#include "entityfactory.hpp"
#include "entitymanager.hpp"
#include "tilefactory.hpp"
#include "tilemanager.hpp"
#include "sfml.hpp"

/** Engine
 * Represents one game state.
 */
class StateManager;
class Engine {
    bool update_pause = false;
    bool running = false;
    SFML & sfml;

    friend StateManager;
    Engine(SFML & sf);
    void init();

    void poll_events();
    void draw(const sf::Color & bgcolor = sf::Color::White);
    void update();

public:
    Camera              camera;
    input::Manager      inputm;
    WorldRender         wrender;
    UIRender            uirender;
    SpriteManager       spritem;
    SpriteFactory       spritef;
    AnimationSystem     anims;
    AnimationFactory    animf;
    EntityFactory       entityf;
    EntitySystem        entitys;
    EntityManager       entitym;
    TileFactory         tilef;
    TileManager         tilem;

    ~Engine() {}
    void run();
    void stop();
    void reset();
};

#endif
