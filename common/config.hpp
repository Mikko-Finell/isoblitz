#ifndef config_hpp
#define config_hpp

// TODO easy
// replace all the defines with the configs

#define WINW 800
#define WINH 800
#define TILEW 128
#define TILEH TILEW / 2
#define COLS_PER_TILE 8
#define ROWS_PER_TILE 8
#define CELLW TILEW / COLS_PER_TILE
#define CELLH TILEH / ROWS_PER_TILE
#define SPRIW 128
#define SPRIH 128
#define HALFW (TILEW * 0.5f)
#define HALFH (TILEH * 0.5f)
#define DIRECTIONS 4
#define EDITOR_VERSION 0
#define TILE_LAYER 0
#define TILE_INDICATOR_LAYER 1
#define ENTITY_LAYER 2
#define ENTITY_INDICATOR_LAYER 3
#define UI_LAYER 4

namespace config {
static constexpr int winw = 800;
static constexpr int winh = 800;
static constexpr int tilew = 128;
static constexpr int tileh = tilew / 2;
static constexpr int cols_per_tile = 8;
static constexpr int rows_per_tile = 8;
static constexpr int cellw = tilew / cols_per_tile;
static constexpr int cellh = tileh / rows_per_tile;
static constexpr int spriw = 128;
static constexpr int sprih = 128;
static constexpr float halfw = (tilew * 0.5f);
static constexpr float halfh = (tileh * 0.5f);
static constexpr int directions = 4;
static constexpr int editor_version = 0;
static constexpr int tile_layer = 0;
static constexpr int tile_indicator_layer = 1;
static constexpr int entity_layer = 2;
static constexpr int entity_indicator_layer = 3;
static constexpr int ui_layer = 4;
static constexpr char spritesheet_file[] = "../sprites/sprites.png";
static constexpr float zoomfactor = 2.0f;
}

#endif
