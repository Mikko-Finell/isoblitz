#include "engine.hpp"
#include <CASE/timer.hpp>
#include <iostream>
using namespace CASE;

Engine::Engine() 
    : camera(window),
     inputm(window),
     wrender(texture),
     uirender(texture),
     animf(wrender),
     tilef(wrender),
     entityf(animf, wrender),
     map(wrender, tilef)
{

    window.create(sf::VideoMode{WINW, WINH}, "Bullet");
    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(60);
    camera.center_window(1920, 1080, WINW, WINH);
    texture.loadFromFile("../sprites/sprites.png");

    inputm.push_context(globctx);
    globctx.bind("quit", [&](){ window.close(); });
    input::Event event{sf::Event::Closed};
    globctx.bind(event, "quit");
    event.set_type(sf::Event::KeyPressed);
    event.set_key(sf::Keyboard::Q);
    globctx.bind(event, "quit");

    event.set_type(sf::Event::KeyPressed);
    event.set_key(sf::Keyboard::S);
    event.set_mod(input::Mod::CTRL, true);
    globctx.bind(event, [&](){
        save(map.filename());
        return true;
    });

    event.set_key(sf::Keyboard::L);
    globctx.bind(event, [&](){
        load(map.filename());
        return true;
    });
    
    input::Event zoom{sf::Event::MouseWheelScrolled};
    globctx.bind(zoom, [&](const input::Event & event){
        constexpr float zoomfactor = 2.0f;
        if (event.get_scroll() > 0) {
            camera.zoom(zoomfactor);
        }
        else {
            camera.zoom(1/zoomfactor);
        }
        return true;
    });

    input::Event move_world{sf::Event::MouseMoved};
    globctx.bind(move_world, [&](const input::Event & event){
        if (inputm.is_button_pressed(sf::Mouse::Middle)) {
            // note: mousedt is the actual change, and so moving the camera by
            // that amount makes it look like the world is scrolling in the 
            // opposite direction, multiply by -1 to make it look like we are
            // moving the world instead of the camera 
            camera.scroll(sf::Vector2f(-1 * event.get_mousedt()));
            return true;
        }
        return false;
    });
}

void Engine::poll_events() {
    inputm.poll_sfevents();
}

void Engine::draw(const sf::Color & bgcolor) {
    window.clear(bgcolor);
    wrender.draw(window);
    uirender.draw(window);
    window.display();
}

void Engine::run() {
    while (window.isOpen()) {
        poll_events();
        draw();
    }
}

void Engine::load(const std::string & filename) {
    if (std::ifstream in{filename, std::ios::binary}; in.good()) {
        std::cout << "Loading " << map.filename() << std::endl;
        camera.deserialize(in);
        map.deserialize(in);
    }
    else {
        std::cerr << "Could not load " << filename << std::endl;
        std::terminate();
    }
}

void Engine::save(const std::string & filename) const {
    if (std::ofstream out{filename, std::ios::binary}; out.good()) {
        std::cout << "Saving " << map.filename() << std::endl;
        camera.serialize(out);
        map.serialize(out);
    }
    else {
        std::cerr << "Could not save " << filename << std::endl;
        std::terminate();
    }
}
