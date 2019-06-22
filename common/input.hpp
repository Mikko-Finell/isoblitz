#ifndef input_hpp
#define input_hpp

#include "coordinate.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <list>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <bitset>
#include <memory>

namespace input {

using hash_t = std::size_t;
enum Mod { CTRL=0, SHIFT=1, ALT=2 };

/**
 * Event
 * Represents one event in the game.
 */
class Event {
    std::bitset<3> mod;
    int type = -1;
    int key = -1;
    int button = -1;
    int scroll = 0;
    hash_t hash = -1;
    Position mousepos_screen;
    Position mousepos_pixel;
    Coordinate mousepos_logic;
    sf::Vector2i mousedt;

    // generate a unique hash based on event type

public:
    hash_t compute_hash() const;
    Event();
    Event(int t);
    Event(const sf::Event & sfevent);
    bool operator==(const Event & other) const;
    hash_t get_hash() const;
    const Position get_mousepos_pixel() const;
    const Position & get_mousepos_screen() const;
    const Coordinate & get_mousepos_logic() const;
    sf::Vector2i get_mousedt() const;
    int get_scroll() const;
    void set_mousepos_screen(const Position & p);
    void set_mousepos_pixel(const Position & p);
    void set_mousepos_logic(const Coordinate & c);
    void set_mousedt(const sf::Vector2i & v);
    void set_type(int t);
    void set_key(int k);
    void set_button(int b);

    // modifier keys control, shift, alt.
    // param b is true if key was down when event created
    void set_mod(Mod m, bool b = true);
};

// Manager //////////////////////////////////////////////////////////////////////

using Callback = std::function<bool(const Event &)>;

class Context;

/**
 * Manager
 * Maintains an inputhandling context stack, and
 * passes sf::Event objects through the stack.
 */
class Manager {
    std::list<Context *> context_queue;

    // contexts is treated as a stack where back=top
    std::list<Context *> contexts;

    std::unordered_map<std::string, Callback> name_to_callback;
    sf::RenderWindow * sfwin = nullptr;

    // value initialization for bool guaranteed to be false
    std::unordered_map<sf::Mouse::Button, bool> button_down;
    std::unordered_map<sf::Keyboard::Key, bool> key_down;

    sf::Vector2f mouse_pos;
    sf::Vector2i mouse_dt;

    std::unique_ptr<Context> globctx;

public:
    ~Manager();
    Manager(sf::RenderWindow & w);
    void set_window(sf::RenderWindow & win);

    void process_event(const sf::Event & sfevent);
    void poll_sfevents();
    void push_context(Context * c);
    void push_context(Context & c);
    void remove_context(Context * c);
    void pop_context();
    Context * get_global_context();

    // creates a trigger bound to a name
    void create_action(const std::string & name, const Callback & callback);
    void create_action(const std::string & n, const std::function<void()> & fn);

    std::optional<Callback> get_action(const std::string & name);
    sf::Vector2f get_mousepos();

    inline bool is_button_pressed(sf::Mouse::Button button) {
        return button_down[button];
    }
    inline bool is_key_pressed(sf::Keyboard::Key key) {
        return key_down[key];
    }
};

// Context //////////////////////////////////////////////////////////////////////

/**
 * Context
 * Manages the relationship between events and actions.
 */
class Context {
    struct EventHasher {
        hash_t operator()(const Event & event) const {
            return event.get_hash();
        }
    };
    std::unordered_map<Event, Callback, EventHasher> event_to_callback;
    std::unordered_map<Event, std::string, EventHasher> event_to_name;
    std::unordered_map<std::string, Callback> name_to_callback;

public:
    Manager * manager = nullptr;

    virtual ~Context();
    void set_manager(Manager * m);
    void remove_manager();
    virtual bool execute(const Event & arg);
    virtual bool execute(const std::string & name);
    void bind(const Event & event, const Callback & callback);
    void bind(const Event & event, const std::function<void()> & fn);
    void bind(const Event & event, const std::string & name);
    void bind(const std::string & name, const Callback & callback);
    void bind(const std::string & name, const std::function<void()> & fn);

    bool is_button_pressed(sf::Mouse::Button button);
    bool is_key_pressed(sf::Keyboard::Key key);
};

} // input

#endif
