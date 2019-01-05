#ifndef system_hpp
#define system_hpp

#include <cassert>

class GameObject;

/**
 * System
 * Abstract baseclass for a system. Systems are responsible
 * for handling GameObjects
 */
class System {
public:
    virtual ~System() {}
    virtual void remove(GameObject *) = 0;
};

/**
 * GameObject
 * Base class for objects that are managed by systems. The
 * point is that objects get a standard way to reference their
 * managing system, eg for removing themself when lifetime ends.
 */
class GameObject {
protected:
    System * system = nullptr;

    // remove ourself from managing system
    virtual void unreg() {
        if (system) {
            system->remove(this);
            system = nullptr;
        }
    }

public:
    virtual ~GameObject() {
        assert(system == nullptr);
        // TODO why not try to unreg here?
    }

    virtual void reg(System * sys) {
        if (sys != nullptr && system != nullptr) {
            // no problem unless trying to reg same obj to multiple systems
            assert(sys == system);
        }
        // TODO if sys==nullptr try to unreg automatically
        //else { unreg(); }
        system = sys;
    }
};

#endif
