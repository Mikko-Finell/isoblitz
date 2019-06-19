#ifndef tileedit_hpp
#define tileedit_hpp

#include "tilemenu.hpp"
#include "tilecursor.hpp"
#include "tilebrush.hpp"
#include "common/input.hpp"
#include "common/engine.hpp"

class TileEdit {
    input::Context cursor_ctx;
    input::Context menu_ctx;
    input::Context brush_ctx;
    TileMenu menu;
    TileCursor cursor;
    TileBrush brush;

public:
    ~TileEdit();
    TileEdit(Engine & engine);
};

#endif 
