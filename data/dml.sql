INSERT INTO Entity (name, sprite_w, sprite_h, sprite_offset_x, sprite_offset_y, 
    tileset_origin_x, tileset_origin_y)
VALUES 
    ("test", 32, 32, 0, 0, 0, 0),
    ("game-ui", 32, 32, 0, 0, 0, 0),
    ("editor-ui", 32, 32, 0, 0, 0, 0),

    -- entities (units)
    ("unit4", 48, 48, 23, 38, 384, 0),
    ("enemy1", 101, 101, 51, 97, 577, 0)
;

INSERT INTO Entity (name, sprite_w, sprite_h, tileset_origin_x, tileset_origin_y)
VALUES 
    -- tiles
    ("test_tiles", 128, 128, 0, 256)
;

INSERT INTO Sprite (entity, name, x, y)
VALUES
    ("test", "hitbox-bg", 64, 128),
    -- game
    ("game-ui", "selection-rect", 256, 128),
    ("game-ui", "tile-indicator", 128, 128),
    -- editor
    ("editor-ui", "tilemenu-bg", 0, 128),
    ("editor-ui", "tilemenu-hovering", 96, 128),
    ("editor-ui", "tilemenu-selected", 64, 160),
    -- units
    ("unit4", "selection", 48, 432),
    ("unit4", "dead", 0, 432),
    ("unit4", "idle-down-right", 0, 0),
    ("unit4", "idle-right", 0, 48),
    ("unit4", "idle-up-right", 0, 96),
    ("unit4", "idle-up", 0, 144),
    ("unit4", "idle-up-left", 0, 192),
    ("unit4", "idle-left", 0, 240),
    ("unit4", "idle-down-left", 0, 288),
    ("unit4", "idle-down", 0, 336),
    -- move
    ("unit4", "move-down-right", 0, 0),
    ("unit4", "move-right", 0, 48),
    ("unit4", "move-up-right", 0, 96),
    ("unit4", "move-up", 0, 144),
    ("unit4", "move-up-left", 0, 192),
    ("unit4", "move-left", 0, 240),
    ("unit4", "move-down-left", 0, 288),
    ("unit4", "move-down", 0, 336),
    -- die
    ("unit4", "die", 0, 384),

    ("enemy1", "idle-down", 0, 0),
    ("enemy1", "idle-up", 0, 144),
    ("enemy1", "idle-right", 0, 48),
    ("enemy1", "idle-left", 0, 240),
    -- move
    ("enemy1", "move-down", 0, 404),
    ("enemy1", "move-up", 0, 505),
    ("enemy1", "move-left", 0, 606),
    ("enemy1", "move-right", 0, 707)
;

INSERT INTO Animation (name, sequence, frames)
VALUES 
    ("unit4", "idle-down-right", 1),
    ("unit4", "idle-right", 1),
    ("unit4", "idle-up-right", 1),
    ("unit4", "idle-up", 1),
    ("unit4", "idle-up-left", 1),
    ("unit4", "idle-left", 1),
    ("unit4", "idle-down-left", 1),
    ("unit4", "idle-down", 1),
    -- move
    ("unit4", "move-down-right", 4),
    ("unit4", "move-right", 4),
    ("unit4", "move-up-right", 4),
    ("unit4", "move-up", 4),
    ("unit4", "move-up-left", 4),
    ("unit4", "move-left", 4),
    ("unit4", "move-down-left", 4),
    ("unit4", "move-down", 4),
    -- die
    ("unit4", "die", 4),

    ("enemy1", "idle-down", 1),
    ("enemy1", "idle-up", 1),
    ("enemy1", "idle-right", 1),
    ("enemy1", "idle-left", 1),
    -- move
    ("enemy1", "move-down", 8),
    ("enemy1", "move-up", 8),
    ("enemy1", "move-left", 8),
    ("enemy1", "move-right", 8)
;

INSERT INTO Tile_Region (name, columns, rows)
VALUES 
    ("test_tiles", 2, 4)
;

INSERT INTO Hitbox (entity, offset_x, offset_y, w, h)
VALUES
    ("unit4", 11, 29, 22, 29),
    ("enemy1", 19, 82, 35, 87)
;

