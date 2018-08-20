#ifndef engine_hpp
#define engine_hpp

#include "camera.hpp"
#include "input.hpp"
#include "rendersystem.hpp"
#include "spritefactory.hpp"
#include "animationfactory.hpp"
#include "tilefactory.hpp"
#include "entityfactory.hpp"
#include "map.hpp"

class Engine {
    virtual void poll_events();
    virtual void draw(const sf::Color & bgcolor = sf::Color::White);

public:
    sf::RenderWindow    window;
    sf::Texture         texture;
    Camera              camera;
    input::Manager      inputm;
    input::Context      globctx;
    WorldRender         wrender;
    UIRender            uirender;
    SpriteFactory       spritef;
    AnimationFactory    animf;
    TileFactory         tilef;
    EntityFactory       entityf;
    Map                 map;

    virtual ~Engine() {}
    Engine();
    void run();

    void load(const std::string & filaname);
    void save(const std::string & filename) const;
};

#endif
