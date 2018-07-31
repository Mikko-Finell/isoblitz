#ifndef __OBSERVER__
#define __OBSERVER__

#include <functional>
#include <list>

namespace bulletimpl {
class __EventBase;
}

class Observer {
    std::list<bulletimpl::__EventBase*> __sub;

public:
    void __set_unsub_hook(bulletimpl::__EventBase * __eventbase) {
        __sub.push_back(__eventbase);
    }
    void unsubscribe(bulletimpl::__EventBase * __eventbase) {
        auto itr = __sub.begin();
        while (itr != __sub.end()) {
            if (*itr == __eventbase) {
                __sub.erase(itr);
                break;
            }
            ++itr;
        }
    }
    virtual ~Observer();
};

namespace bulletimpl {
class __EventBase {
public:
    virtual void unsubscribe(Observer *) = 0;
    virtual ~__EventBase() {}
};
}

Observer::~Observer() {
    for (auto __eventbase : __sub) {
        __eventbase->unsubscribe(this);
    }
}

template<typename... Args>
class Event final : bulletimpl::__EventBase {
    using fn_type = std::function<void(Args...)>;
    std::list<std::pair<Observer*, fn_type*>> observers;
    std::list<fn_type> callbacks;

public:
    void subscribe(Observer & obs, fn_type & callback) {
        observers.push_back(std::make_pair(&obs, &callback));
        obs.__set_unsub_hook(this);
    }

    void subscribe(const fn_type & callback) {
        callbacks.push_back(callback);
    }

    void unsubscribe(Observer * obs) override {
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
