#ifndef ui_hpp
#define ui_hpp

#include "input.hpp"
#include "sprite.hpp"
#include "spritemanager.hpp"
#include "rendersystem.hpp"
#include "observer.hpp"
#include <list>
#include <functional>

namespace UI {
class Container;
class Element : public Observer {
    friend Container;

    using ID = int;
    ID _id = 0;

    Element();
    virtual void _activate();
    virtual void _hover();
    virtual void _init();

public:
    Signal<const ID> clicked;

    struct {
        Sprite * idle = nullptr;
        Sprite * hovering = nullptr;
        Sprite * activated = nullptr;
    } sprite;

    virtual ~Element();
    std::function<void()> on_activate;
    std::function<void()> on_hover;
    std::function<void()> on_init;

    void activate();
    void hover();
    void init();

    inline const ID id(const ID & i = 0) {
        if (i != 0) {
            _id = i;
        }
        return _id;
    }
};

class Container : Observer {
    std::list<Element> elements;
    std::list<Element *> element_ptrs;
    SpriteManager & spritem;

protected:
    virtual void cleanup();

public:
    Sprite * background = nullptr;
    std::function<void(Element &)> on_hover;
    virtual ~Container();
    Container(input::Manager & inputm, SpriteManager & sm);
    virtual void add_element(Element * element);
};
} // UI

#endif
