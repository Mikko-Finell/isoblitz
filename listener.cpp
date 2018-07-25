#include "listener.hpp"
#include "emitter.hpp"

using namespace Event;

void Listener::recvevent(Event event) {
}

Listener::~Listener() {
    for (auto emitter : emitters) {
        emitter->removelistener(this);
    }
}
