#ifndef __SHELL__
#define __SHELL__

#include <functional>
#include <mutex>
#include <thread>
#include <list>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include "common/observer.hpp"

class Shell {
    std::mutex mutex;
    std::list<std::function<void()>> deferred_signals;

public:
    struct {
        Signal<> quit;
        Signal<const std::string &> set_spritesheet;
        Signal<const sf::Color &> set_bgcolor;

        Signal<bool> set_blocked;
        Signal<const sf::Vector2i &> set_sprite;
        Signal<const sf::Vector2f &> paint;

        Signal<const std::string &> save;
        Signal<const std::string &> load;
        Signal<const std::string &> newmap;
        Signal<const std::string &> set_mapname;
    } signal;

    void defer(const std::function<void()> & event);
    void emit_signals();
    void launch();
    ~Shell();
};

#endif
