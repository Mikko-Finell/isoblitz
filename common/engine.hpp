#ifndef engine_hpp
#define engine_hpp

#include "observer.hpp"
#include "camera.hpp"
#include "input.hpp"
#include "rendersystem.hpp"
#include "spritemanager.hpp"
#include "spritefactory.hpp"
#include "animationfactory.hpp"
#include "entitymanager.hpp"
#include "entityfactory.hpp"
#include "tilefactory.hpp"
#include "tilemanager.hpp"
#include "selectionmanager.hpp"
#include "movementsystem.hpp"
#include "pathmanager.hpp"
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
    EntityManager       entitym;
    TileFactory         tilef;
    MovementSystem      moves;
    TileManager         tilem;
    PathManager         pathm;
    SelectionManager    selectm;

    struct {
        Signal<float> update;
    } signals;

    ~Engine() {}
    void run();
    void stop();
    void reset();
    void load(const std::string & filename = "tmp.isoblitz", const std::string & path = "../maps/");
    void save(const std::string & filename = "tmp.isoblitz", const std::string & path = "../maps/");
};

#endif
