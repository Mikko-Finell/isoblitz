#ifndef __LISTENER__
#define __LISTENER__

#include <set>
#include "event.hpp"

class Emitter;

class Listener {
    std::set<Emitter*> emitters;
    
public:
    virtual void recvevent(Event event);
    virtual ~Listener();
};

#endif
