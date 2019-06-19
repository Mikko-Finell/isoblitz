#ifndef entitymenu_hpp
#define entitymenu_hpp

#include "editormenu.hpp"
#include "common/ui.hpp"
#include "common/engine.hpp"

class EntityMenuItem : public EditorMenuItem {
public:
    using EditorMenuItem::EditorMenuItem;
};

class EntityMenu : public EditorMenu {
    void cleanup() override;

public:
    Signal<std::string> entity_selected;

    virtual ~EntityMenu();
    EntityMenu(Engine & engine);
};

#endif
