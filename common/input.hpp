#ifndef __INPUT__
#define __INPUT__

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace input {

enum class Mod { CTRL, SHIFT, ALT, ALL };

class Event {
public:
    using hash_t = std::size_t;

private:
    bool ctrl = false, shift = false, alt = false;
    bool mod = false;
    int type = -1;
    int key = -1;
    int button = -1;
    hash_t hash = -1;
    sf::Vector2f mousepos;

    hash_t compute_hash() const;

public:
    Event();
    Event(int t);
    Event(const sf::Event & sfevent);
    bool operator==(const Event & other) const;
    hash_t get_hash() const;
    sf::Vector2f get_mousepos() const;
    void set_mousepos(const sf::Vector2f & v);
    void set_type(int t);
    void set_key(int k);
    void set_button(int b);
    void set_mod(Mod m, bool b = true);
};

using Callback = std::function<bool(const Event &)>;

class Context;

class Manager {
    std::vector<Context *> contexts;
    std::unordered_map<std::string, Callback> name_to_callback;
    sf::RenderWindow * sfwin = nullptr;

public:
    virtual ~Manager() {}
    void set_window(sf::RenderWindow & win);
    virtual void process_event(const sf::Event & sfevent);
    virtual void poll_sfevents();
    void push_context(Context * c);
    void remove_context(Context * c);

    void create_action(const std::string & name, const Callback & callback);
    Callback get_action(const std::string & name);
};

class Context {
    struct EventHasher {
        Event::hash_t operator()(const Event & event) const {
            return event.get_hash();
        }
    };
    std::unordered_map<Event, Callback, EventHasher> event_to_callback;
    std::unordered_map<Event, std::string, EventHasher> event_to_name;
    std::unordered_map<std::string, Callback> name_to_callback;

public:
    Manager * manager = nullptr;

    virtual ~Context() {}
    void set_manager(Manager * m);
    void remove_manager();
    virtual bool execute(const Event & arg);
    virtual bool execute(const std::string & name);
    void bind(const Event & event, const Callback & callback);
    void bind(const Event & event, const std::function<void()> & fn);
    void bind(const Event & event, const std::string & name);
    void create_action(const std::string & name, const Callback & callback);
    void create_action(const std::string & n, const std::function<void()> & fn);
};

} // input

#endif
