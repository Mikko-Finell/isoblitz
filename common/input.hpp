#ifndef __INPUT__
#define __INPUT__

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

enum class Mod { CTRL, SHIFT, ALT, ALL };

class Event {
    bool ctrl = false, shift = false, alt = false;
    bool mod = false;
    int type = -1;
    int key = -1;
    int button = -1;
    std::size_t hash = -1;
    sf::Vector2f mousepos;

    std::size_t compute_hash() const;

public:
    Event();
    Event(int t);
    Event(const sf::Event & sfevent);
    bool operator==(const Event & other) const;
    std::size_t get_hash() const;
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

public:
    virtual ~Manager() {}
    virtual void process(sf::RenderWindow & w);
    void push_context(Context * c);
    void remove_context(Context * c);

    void create_action(const std::string & name, const Callback & callback);
    Callback get_action(const std::string & name);
};

class Context {
    struct EventHasher {
        std::size_t operator()(const Event & event) const {
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
    void bind(const Event & event, const std::string & name);
    void create_action(const std::string & name, const Callback & callback);
};

#endif
