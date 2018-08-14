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
        //case sf::Event::MouseMoved:
            //mousedt.x = sfevent.mouseMove.x;
            //mousedt.y = sfevent.mouseMove.y;
            //break;
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

sf::Vector2i Event::get_mousepos() const {
    return mousepos;
}

sf::Vector2i Event::get_mousedt() const {
    return mousedt;
}

void Event::set_mousepos(const sf::Vector2i & v) {
    mousepos = v;
}

void Event::set_mousedt(const sf::Vector2i & v) {
    mousedt = v;
}

// Manager

Manager::Manager() {
}

Manager::Manager(sf::RenderWindow & w) {
    set_window(w);
}

void Manager::set_window(sf::RenderWindow & win) {
    sfwin = &win;
}

void Manager::process_event(const sf::Event & sfevent) {
    switch (sfevent.type) {
        case sf::Event::KeyPressed:
            key_down[sfevent.key.code] = true;
            break;
        case sf::Event::KeyReleased:
            key_down[sfevent.key.code] = false;
            break;
        case sf::Event::MouseButtonPressed:
            button_down[sfevent.mouseButton.button] = true;
            break;
        case sf::Event::MouseButtonReleased:
            button_down[sfevent.mouseButton.button] = false;
            break;
        case sf::Event::MouseMoved:
            mouse_dt.x = mouse_pos.x - sfevent.mouseMove.x;
            mouse_dt.y = mouse_pos.y - sfevent.mouseMove.y;
            mouse_pos.x = sfevent.mouseMove.x;
            mouse_pos.y = sfevent.mouseMove.y;
        default:
            break;
    }

    assert(sfwin);
    Event arg{sfevent};
    arg.set_mousepos(mouse_pos);
    arg.set_mousedt(mouse_dt);
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

void Manager::push_context(Context & context) {
    push_context(&context);
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

void 
Manager::create_action(const std::string & n, const std::function<void()> & fn){
    create_action(n, [fn](const Event &){ fn(); return true; });
}

std::optional<Callback> Manager::get_action(const std::string & name) {
    if (auto itr = name_to_callback.find(name); itr != name_to_callback.end()) {
        return (*itr).second;
    }
    return std::nullopt;
}

sf::Vector2f Manager::get_mousepos() {
    auto pos = sfwin->mapPixelToCoords(sf::Mouse::getPosition(*sfwin));
    return pos;
}

// Context

void Context::set_manager(Manager * m) {
    manager = m;
}

void Context::remove_manager() {
    assert(manager);
    manager->remove_context(this);
    manager = nullptr;
}

bool Context::execute(const Event & arg) {
    auto itr = event_to_callback.find(arg);
    if (itr != event_to_callback.end()) {
        auto action = itr->second;
        if (action(arg)) { // actions cal pass event through by return false
            return true;
        }
    }
    auto itr1 = event_to_name.find(arg);
    if (itr1 != event_to_name.end()) {
        auto & name = itr1->second;
        return execute(name);
    }
    return false;
}

bool Context::execute(const std::string & name) {
    assert(manager);
    if (auto itr = name_to_callback.find(name); itr != name_to_callback.end()) {
        Event null_event;
        auto & callback = itr->second;
        return callback(null_event);
    }
    if (auto action = manager->get_action(name)) {
        return (*action)(Event{});
    }
    return false;
}

void Context::bind(const Event & event, const Callback & callback) {
    event_to_callback[event] = callback;
}

void Context::bind(const Event & event, const std::function<void()> & fn) {
    bind(event, [fn](const Event &){ fn(); return true; });
}

void Context::bind(const Event & event, const std::string & name) {
    event_to_name[event] = name;
}

void Context::bind(const std::string & name, const std::function<void()> & fn) {
    name_to_callback[name] = [fn](const Event &){ fn(); return true; };
}
