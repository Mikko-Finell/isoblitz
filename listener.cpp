#include "listener.hpp"
#include "emitter.hpp"

void Listener::recvevent(const Event & event) {
}

Listener::~Listener() {
    for (auto emitter : emitters) {
        emitter->removelistener(this);
    }
}
