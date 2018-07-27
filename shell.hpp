#ifndef __SHELL__
#define __SHELL__

#include <mutex>
#include <thread>
#include <vector>
#include "event.hpp"
#include "emitter.hpp"

class Shell : public Emitter {
    std::mutex mutex;
    std::thread thread;
    std::vector<Event> events;

public:
    void store_event(const Event & event);
    void emit_events();
    void launch();
    ~Shell();
};

#endif
