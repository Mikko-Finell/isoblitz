#ifndef tilemenu_hpp
#define tilemenu_hpp

#include "editormenu.hpp"
#include "common/ui.hpp"
#include "common/engine.hpp"

class TileMenuItem : public EditorMenuItem {
public:
    using EditorMenuItem::EditorMenuItem;
};

class TileMenu : public EditorMenu {
    void cleanup() override;

public:
    Signal<Tile::ID> tile_selected;

    virtual ~TileMenu();
    TileMenu(Engine & engine);
};

#endif
