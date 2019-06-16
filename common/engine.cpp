#include "engine.hpp"
#include "util.hpp"
#include <CASE/timer.hpp>
#include <thread>
#include <chrono>
#include <iostream>

void Engine::init() {
    window.create(sf::VideoMode{WINW, WINH}, "Bullet");
    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(60);
    camera.center_window(1920, 1080, WINW, WINH);

    texture.loadFromFile(config::spritesheet_file);

    inputm.set_global_context(globctx);
    globctx.bind("quit", [&](){ window.close(); });
    input::Event event{sf::Event::Closed};
    globctx.bind(event, "quit");
    event.set_type(sf::Event::KeyPressed);
    event.set_key(sf::Keyboard::Q);
    globctx.bind(event, "quit");

    event.set_key(sf::Keyboard::P);
    globctx.bind(event, [&](){ pause = !pause; });

    event.set_key(sf::Keyboard::Space);
    globctx.bind(event, [&](){
        for (auto entity : entitym.get_all()) {
            std::cout << entity->info() << std::endl;
        }
    });


    event.set_key(sf::Keyboard::L);
    globctx.bind(event, [&](){
        load(map.filename());
        return true;
    });

    input::Event zoom{sf::Event::MouseWheelScrolled};
    globctx.bind(zoom, [&](const input::Event & event){
        if (event.get_scroll() > 0) {
            camera.zoom(config::zoomfactor);
        }
        else {
            camera.zoom(1/config::zoomfactor);
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

Engine::Engine() 
    : camera(window),
     inputm(window),
     wrender(texture),
     uirender(texture),
     spritem(),
     spritef(spritem),
     anims(),
     animm(spritem),
     animf(animm, anims, spritef),
     entitys(),
     entitym(animm),
     entityf(animf, entitym, entitys),
     tilem(spritem),
     tilef(tilem, spritef),
     map(wrender, tilef)
{
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

void Engine::update() {
    if (pause == false) {
        entitys.update(16);
    }
    anims.update(16);
}

void Engine::run() {
    while (is_running()) {
        poll_events();
        update();
        draw();
    }
}

void Engine::reset() {
    map.clear();
    entitym.clear();
}

void Engine::load(const std::string & filename) {
    if (std::ifstream in{filename, std::ios::binary}; in.good()) {
        CASE::ScopeTimer t{"Loading " + map.filename()};
        camera.deserialize(in);
        map.deserialize(in);
        entitym.deserialize(&entityf, in);
    }
    else {
        std::cerr << "Could not load " << filename << std::endl;
    }
}

void Engine::save(const std::string & filename) const {
    if (std::ofstream out{filename, std::ios::binary}; out.good()) {
        CASE::ScopeTimer t{"Saving " + map.filename()};
        camera.serialize(out);
        map.serialize(out);
        entitym.serialize(out);
    }
    else {
        std::cerr << "Could not save " << filename << std::endl;
        std::terminate();
    }
}

bool Engine::is_running() const {
    return window.isOpen();
}
