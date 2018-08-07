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

// Signal
/*
template<typename... Args>
void Signal<Args...>::add_observer(Observer * obs, const fn_type & callback) {
    stored_callbacks[obs] = callback;
    observers.push_front(std::make_pair(obs, &stored_callbacks[obs]));
    obs->__set_unsub_hook(this);
}

template<typename... Args>
template<class T>
void Signal<Args...>::add_observer(T & obs, void (T::* pm)(Args...)) {
    auto memfn = std::mem_fn(pm);
    auto action = [memfn, &obs](Args... args){ memfn(obs, args...); };
    add_observer(&obs, action);
}

template<typename... Args>
template<class T>
void Signal<Args...>::add_observer(T * obs, void (T::* pm)(Args...)) {
    add_observer(*obs, pm);
}


template<typename... Args>
void Signal<Args...>::add_callback(const fn_type & callback) {
    callbacks.push_front(callback);
}

template<typename... Args>
void Signal<Args...>::remove_observer(Observer * obs) {
    for (auto & pair : observers) {
        if (pair.first == obs) {
            pair = std::make_pair(nullptr, nullptr);
            break;
        }
    }
}

template<typename... Args>
void Signal<Args...>::operator()(Args... args) {
    for (auto & callback : callbacks) {
        callback(args...);
    }
    auto itr = observers.begin();
    while (itr != observers.end()) {
        auto & pair = *itr;
        if (pair.first == nullptr) {
            itr = observers.erase(itr);
        }
        else {
            auto & action = *pair.second;
            action(args...);
            ++itr;
        }
    }
}

template<typename... Args>
Signal<Args...>::~Signal() {
    for (auto & pair : observers) {
        if (pair.first) {
            pair.first->unsubscribe(this);
        }
    }
}
*/
