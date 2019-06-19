#ifndef entityedit_hpp
#define entityedit_hpp

#include "entitymenu.hpp"
#include "entitycursor.hpp"
#include "entitybrush.hpp"
#include "common/input.hpp"
#include "common/engine.hpp"

class EntityEdit {
    input::Context cursor_ctx;
    input::Context menu_ctx;
    input::Context brush_ctx;
    EntityMenu menu;
    EntityCursor cursor;
    EntityBrush brush;

public:
    ~EntityEdit();
    EntityEdit(Engine & engine);
};

#endif 
