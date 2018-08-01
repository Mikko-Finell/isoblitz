#ifndef __OBSERVER__
#define __OBSERVER__

#include <list>
#include <functional>
#include <unordered_map>

class Observer;

namespace impl {
class __EventBase {
public:
    virtual void remove_observer(Observer *) = 0;
    virtual ~__EventBase() {}
};
}

class Observer {
    std::list<impl::__EventBase*> __sub;

public:
    void __set_unsub_hook(impl::__EventBase * __eventbase) {
        __sub.push_front(__eventbase);
    }
    void unsubscribe(impl::__EventBase * __eventbase) {
        auto itr = __sub.begin();
        while (itr != __sub.end()) {
            if (*itr == __eventbase) {
                __sub.erase(itr);
                break;
            }
            ++itr;
        }
    }
    virtual ~Observer() {
        for (auto __eventbase : __sub) {
            __eventbase->remove_observer(this);
        }
    }
};

template<typename... Args>
class Event final : impl::__EventBase {
    using fn_type = std::function<void(Args...)>;
    std::unordered_map<Observer*, fn_type> stored_callbacks;
    std::list<std::pair<Observer*, fn_type*>> observers;
    std::list<fn_type> callbacks;

public:
    void add_observer(Observer * obs, const fn_type & callback) {
        stored_callbacks[obs] = callback;
        observers.push_front(std::make_pair(obs, &stored_callbacks[obs]));
        obs->__set_unsub_hook(this);
    }

    template<class T>
    void add_observer(T & obs, void (T::* pm)(Args...)) {
        auto memfn = std::mem_fn(pm);
        auto action = [memfn, &obs](Args... args){ memfn(obs, args...); };
        add_observer(&obs, action);
    }

    inline void add_observer(Observer & obs, const fn_type & callback) {
        add_observer(&obs, callback);
    }

    void add_callback(const fn_type & callback) {
        callbacks.push_front(callback);
    }

    void remove_observer(Observer * obs) override {
        for (auto & pair : observers) {
            if (pair.first == obs) {
                pair = std::make_pair(nullptr, nullptr);
                break;
            }
        }
    }

    void operator()(Args... args) {
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

    ~Event() {
        for (auto & pair : observers) {
            if (pair.first) {
                pair.first->unsubscribe(this);
            }
        }
    }
};

#endif
