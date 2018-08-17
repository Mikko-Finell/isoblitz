#include "observer.hpp"

void Observer::__set_unsub_hook(impl::SignalBase * signalbase) {
    sub.push_front(signalbase);
}

void Observer::unsubscribe(impl::SignalBase * signalbase) {
    auto itr = sub.begin();
    while (itr != sub.end()) {
        if (*itr == signalbase) {
            itr = sub.erase(itr);
        }
        ++itr;
    }
}

Observer::~Observer() {
    for (auto signalbase : sub) {
        signalbase->remove_observer(this);
    }
}
