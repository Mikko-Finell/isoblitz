#include <cassert>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "input.hpp"

using namespace input;

Event::hash_t Event::compute_hash() const {
    hash_t _hash = 1000000 * type;
    switch (type) {
        case sf::Event::KeyPressed:
            _hash += key;
            break;
        case sf::Event::KeyReleased:
            _hash += key + 100000;
            break;
        case sf::Event::MouseButtonPressed:
            _hash += button + 200000;
            break;
        case sf::Event::MouseButtonReleased:
            _hash += button + 300000;
            break;
        default:
            break;
    }
    _hash += mod * ctrl * 10000;
    _hash += mod * shift * 20000;
    _hash += mod * alt * 30000;
    return _hash;
}

Event::Event() {
}

Event::Event(int t) {
    set_type(t);
}

Event::Event(const sf::Event & sfevent) {
    set_type(sfevent.type);
    bool checkmod = false;
    switch (type) {
        case sf::Event::KeyPressed:
            key = sfevent.key.code;
            checkmod = true;
            break;
        case sf::Event::KeyReleased:
            key = sfevent.key.code;
            checkmod = true;
            break;
        case sf::Event::MouseButtonPressed:
            button = sfevent.mouseButton.button;
            break;
        case sf::Event::MouseButtonReleased:
            button = sfevent.mouseButton.button;
            break;
        default:
            break;
    }
    if (checkmod) {
        ctrl = sfevent.key.control;
        shift = sfevent.key.shift;
        alt = sfevent.key.alt;
        mod = ctrl || shift || alt;
    }
    hash = compute_hash();
}

void Event::set_type(int t) {
    type = t;
    hash = compute_hash();
}

void Event::set_key(int k) {
    key = k;
    hash = compute_hash();
}

void Event::set_button(int b) {
    button = b;
    hash = compute_hash();
}

void Event::set_mod(Mod m, bool b) {
    switch (m) {
        case Mod::CTRL:
            ctrl = b;
            break;
        case Mod::SHIFT:
            shift = b;
            break;
        case Mod::ALT:
            alt = b;
            break;
        default:
            ctrl = shift = alt = b;
    }
    mod = ctrl || shift || alt;
    hash = compute_hash();
}

bool Event::operator==(const Event & other) const {
    return this->get_hash() == other.get_hash();
}

Event::hash_t Event::get_hash() const {
    return hash;
}

sf::Vector2f Event::get_mousepos() const {
    return mousepos;
}

void Event::set_mousepos(const sf::Vector2f & v) {
    mousepos = v;
}

// Manager

void Manager::set_window(sf::RenderWindow & win) {
    sfwin = &win;
}

void Manager::process_event(const sf::Event & sfevent) {
    assert(sfwin);
    Event arg{sfevent};
    arg.set_mousepos(sfwin->mapPixelToCoords(sf::Mouse::getPosition(*sfwin)));
    auto itr = contexts.rbegin();
    while (itr != contexts.rend()) {
        auto context = *itr;
        if (!context) {
            std::advance(itr, 1);
            contexts.erase(std::next(itr).base());
        }
        else if (context->execute(arg)) {
            break;
        }
        else {
            ++itr;
        }
    }
}

void Manager::poll_sfevents() {
    assert(sfwin);
    sf::Event sfevent;
    while (sfwin->pollEvent(sfevent)) {
        process_event(sfevent);
    }
}

void Manager::push_context(Context * context) {
    contexts.push_back(context);
    context->set_manager(this);
}

void Manager::remove_context(Context * context) {
    auto itr = contexts.begin();
    while (itr != contexts.end()) {
        if (*itr == context) {
            *itr = nullptr;
        }
        ++itr;
    }
}

void Manager::create_action(const std::string & name, const Callback & callback)
{
    name_to_callback[name] = callback;
}

Callback Manager::get_action(const std::string & name) {
    auto itr = name_to_callback.find(name);
    if (itr != name_to_callback.end()) {
        return (*itr).second;
    }
    throw std::logic_error{"Nonexistent action " + name};
}

// Context

void Context::set_manager(Manager * m) {
    manager = m;
}

void Context::remove_manager() {
    manager->remove_context(this);
    manager = nullptr;
}

bool Context::execute(const Event & arg) {
    auto itr = event_to_callback.find(arg);
    if (itr != event_to_callback.end()) {
        auto action = itr->second;
        if (action(arg)) {
            return true;
        }
    }
    auto itr1 = event_to_name.find(arg);
    if (itr1 != event_to_name.end()) {
        auto & name = itr1->second;
        return execute(name);
    }
    /*else {
        try {
            auto callback = manager->get_action(arg); // not implemented
            return callback(arg);
        }
        catch (...) {
            return false;
        }
    }*/
    return false;
}

void Context::bind(const Event & event, const std::string & name) {
    auto itr = name_to_callback.find(name);
    if (itr != name_to_callback.end()) {
        bind(event, (*itr).second);
    }
    else {
        try {
            bind(event, manager->get_action(name));
        }
        catch (...) {
            std::string er = "Bind event to nonexistent callback " + name;
            throw std::logic_error{er};
        }
    }
}

void Context::create_action(const std::string & name, const Callback & callback)
{
    name_to_callback[name] = callback;
}

bool Context::execute(const std::string & name) {
    Event null_event;
    auto itr = name_to_callback.find(name);
    if (itr != name_to_callback.end()) {
        auto & callback = itr->second;
        return callback(null_event);
    }
    else {
        try {
            auto callback = manager->get_action(name);
            return callback(null_event);
        }
        catch (...) {
            std::string er = "Execute nonexistent callback " + name;
            throw std::logic_error{er};
        }
    }
}

void Context::bind(const Event & event, const Callback & callback) {
    std::cout << "binding event " << event.get_hash() << "\n";
    event_to_callback[event] = callback;
}

void Context::bind(const Event & event, const std::function<void()> & fn) {
    bind(event, [fn](const Event &){ fn(); return true; });
}

void Context::create_action(const std::string & name, const std::function<void()> & fn) {
    create_action(name, [fn](const Event &){ fn(); return true; });
}
