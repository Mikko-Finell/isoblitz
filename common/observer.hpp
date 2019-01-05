#ifndef observer_hpp
#define observer_hpp

#include <cassert>
#include <list>
#include <functional>
#include <map>
#include <unordered_map>
#include <unordered_set>

class Observer;

/**
 * ISignal
 * Base class for Signal interface, don't use outside here
 */
class ISignal {
public:
    virtual void remove_observer(Observer *) = 0;
    virtual ~ISignal() {}
};

/**
 * Observer
 * Listens to signals.
 */
class Observer {
    std::unordered_set<ISignal *> sub;

public:
    Observer() {
    }

    Observer(const Observer & other) {
        operator=(other);
    }

    Observer & operator=(const Observer & other) {
        if (other.sub.empty() == false) {
            throw std::logic_error{"Copy observer with active subscriptions"};
        }
        return *this;
    }

    Observer & operator=(Observer && other) {
        if (other.sub.empty() == false) {
            throw std::logic_error{"Copy observer with active subscriptions"};
        }
        return *this;
    }

    void __set_unsub_hook(ISignal * signal) {
        assert(sub.insert(signal).second);
    }

    void unsubscribe(ISignal * signal) {
        sub.erase(signal);
    }

    virtual ~Observer() {
        for (auto signal : sub) {
            signal->remove_observer(this);
        }
    }
};

/**
 * Signal
 *
template<typename... Args>
class Signal final : ISignal {
    using fn_type = std::function<void(Args...)>;
    //[std::unordered_map<Observer *, fn_type> owned_callbacks;
    std::list<std::pair<Observer *, fn_type>> observers;
    std::list<std::pair<std::string, fn_type>> named_callbacks;

public:
    Signal() {
    }

    Signal(const Signal & other) {
        operator=(other);
    }

    Signal & operator=(const Signal & other) {
        if (other.observers.empty() == false) {
            throw std::logic_error{"Copy signal with active observers"};
        }
        return *this;
    }

    Signal & operator=(Signal && other) {
        if (other.observers.empty() == false) {
            throw std::logic_error{"Copy signal with active observers"};
        }
        return *this;
    }

    void add_observer(Observer * obs, const fn_type & callback) {
        //owned_callbacks[obs] = callback;
        observers.push_front(std::make_pair(obs, callback));
        obs->__set_unsub_hook(this);
    }

    template<class T>
    void add_observer(T * obs, void (T::* pm)(Args...)) {
        add_observer(*obs, pm);
    }
 
    inline void add_observer(Observer & obs, const fn_type & callback) {
        add_observer(&obs, callback);
    }

    template<class T>
    void add_observer(T & obs, void (T::* pm)(Args...)) {
        auto memfn = std::mem_fn(pm);
        auto action = [memfn, &obs](Args... args){ memfn(obs, args...); };
        add_observer(&obs, action);
    }

    void remove_observer(Observer * obs) override {
        for (auto & pair : observers) {
            if (pair.first == obs) {
                pair.first = nullptr;
                break;
            }
        }
    }

    void add_callback(const std::string & name, const fn_type & callback) {
        named_callbacks.push_back(std::make_pair(name, callback));
    }

    void remove_callback(const std::string & name) {
        for (auto & pair : named_callbacks) {
            if (pair.first == name) {
                pair.first.clear();
                break;
            }
        }
    }

    void operator()(Args... args) {

        if (auto itr = named_callbacks.begin(); itr != named_callbacks.end()) {
            while (itr != named_callbacks.end()) {
                auto & pair = *itr;
                if (pair.first.empty()) {
                    itr = named_callbacks.erase(itr);
                }
                else {
                    auto & callback = pair.second;
                    callback(args...);
                    ++itr;
                }
            }
        }

        auto itr = observers.begin();
        while (itr != observers.end()) {
            auto & pair = *itr;
            if (pair.first == nullptr) {
                itr = observers.erase(itr);
            }
            else {
                auto & callback = pair.second;
                callback(args...);
                ++itr;
            }
        }
    }

    ~Signal() {
        for (auto & pair : observers) {
            if (pair.first) {
                pair.first->unsubscribe(this);
            }
        }
    }
};

#endif
