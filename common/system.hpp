#ifndef system_hpp
#define system_hpp

#include <cassert>

class GameObject;

class System {
public:
    virtual ~System() {}
    virtual void remove(GameObject *) = 0;
};

class GameObject {
protected:
    System * system = nullptr;

    virtual void unreg() {
        if (system) {
            system->remove(this);
            system = nullptr;
        }
    }

public:
    virtual ~GameObject() {
        assert(system == nullptr);
    }

    virtual void reg(System * sys) {
        if (sys != nullptr && system != nullptr) {
            // no problem unless trying to reg same obj to multiple systems
            assert(sys == system);
        }
        system = sys;
    }
};

#endif
