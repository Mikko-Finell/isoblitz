#ifndef __SHELL__
#define __SHELL__

#include <functional>
#include <mutex>
#include <thread>
#include <list>
#include "common/observer.hpp"

class Shell {
    std::mutex mutex;
    std::thread thread;
    std::list<std::function<void()>> deferred_events;

public:
    struct {
        Event<> quit;
        Event<const std::string &> set_spritesheet;

        Event<bool> set_blocked;
        Event<const Coordinate &> set_sprite;

        Event<const std::string &> save;
        Event<const std::string &> load;
        Event<const std::string &> newmap;
        Event<const std::string &> set_mapname;
    } events;

    void defer(const std::function<void()> & event);
    void emit_events();
    void launch();
    ~Shell();
};

#endif
