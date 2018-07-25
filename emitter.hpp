#ifndef __EMITTER__
#define __EMITTER__

#include <set>
#include "event.hpp"

class Listener;

class Emitter {
    std::set<Listener*> listeners;
    
protected:
    void emit(Event event);
    
public:
    void addlistener(Listener * listener);
    void removelistener(Listener * listener);
    virtual ~Emitter();
};

#endif
