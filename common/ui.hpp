#ifndef ui_hpp
#define ui_hpp

#include "engine.hpp"
#include "input.hpp"
#include "sprite.hpp"
#include "spritemanager.hpp"
#include "rendersystem.hpp"
#include "observer.hpp"
#include <list>
#include <functional>

namespace UI {
class Element : public Observer {
public:
    using ID = int;
    ID _id = 0;

    Signal<const ID> clicked;
    bool activated = false, hovering = false;

    struct {
        Sprite idle;
        Sprite hovering;
        Sprite activated;
    } sprite;

    std::function<void()> on_activate;
    std::function<void()> on_hover;
    std::function<void()> on_init;

    virtual ~Element();
    Element();

    virtual void activate();
    virtual void init();
    virtual void update_mousepos(const Position & pos);
    virtual void click(const Position & pos);
    virtual bool contains(const Position & pos);

    inline const ID id(const ID & i = 0) {
        if (i != 0) {
            _id = i;
        }
        return _id;
    }
};

class Container : public Observer {
protected:
    std::list<Element> elements;
    std::list<Element *> element_ptrs;
    Engine & engine;

    virtual void cleanup();

public:
    Sprite background;
    std::function<void(Element &)> on_hover;
    virtual ~Container();
    Container(Engine & eng);
    virtual void add_element(Element * element);
    virtual void update_mousepos(const Position & pos);
    virtual bool contains(const Position & pos);
    virtual bool click(const Position & pos);
};
} // UI

#endif
