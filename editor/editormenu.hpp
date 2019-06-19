#ifndef editormenu_hpp
#define editormenu_hpp

#include "common/ui.hpp"

class EditorMenuItem : public UI::Element {
protected:
    virtual void activate() override;
    virtual void update_mousepos(const Position & pos) override;
    virtual bool contains(const Position & pos) override;
    virtual void click(const Position & pos) override;

public:
    virtual ~EditorMenuItem();
    using UI::Element::Element;
};

class EditorMenu : public UI::Container {
public:
    virtual ~EditorMenu();
    using UI::Container::Container;
};

#endif
