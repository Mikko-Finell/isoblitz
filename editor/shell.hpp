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
    std::list<std::function<void()>> deferred_signals;

public:
    struct {
        Signal<> quit;
        Signal<const std::string &> set_spritesheet;

        Signal<bool> set_blocked;
        Signal<const Coordinate &> set_sprite;

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
