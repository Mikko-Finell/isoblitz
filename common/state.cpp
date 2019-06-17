#include "state.hpp"
#include "config.hpp"

StateManager::~StateManager() {
}

StateManager::StateManager() {
    sfml.window.create(sf::VideoMode{config::winw, config::winh}, "Bullet");
    sfml.window.setKeyRepeatEnabled(false);
    sfml.window.setFramerateLimit(60);
    sfml.texture.loadFromFile(config::spritesheet_file);
}

StateManager & StateManager::get_manager() {
    static StateManager instance;
    return instance;
}

Engine & StateManager::create(const std::string & name) {
    auto & instance = get_manager();
    std::unique_ptr<Engine> engineptr{new Engine{instance.sfml}};
    auto pair = instance.states.emplace(name, std::move(engineptr));
    Engine & engine = *instance.states.at(name);
    engine.init();
    return engine;
}

Engine & StateManager::get_state(const std::string & name) {
    auto & instance = get_manager();
    Engine & engine = *instance.states.at(name);
    return engine;
}

void StateManager::run(const std::string & name) {
    get_state(name).run();
}

void StateManager::terminate() {
    auto & states = get_manager().states;
    for (auto & pair : states) {
        pair.second->stop();
    }
}
