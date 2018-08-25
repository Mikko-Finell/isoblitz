INSERT INTO Entity (name, sprite_w, sprite_h, sprite_offset_x, sprite_offset_y, 
    tileset_origin_x, tileset_origin_y)
VALUES 
    ("test", 32, 32, 0, 0, 0, 0),
    ("game-ui", 128, 128, 0, 0, 0, 0),
    ("editor-ui", 64, 64, 0, 0, 0, 0),
    ("unit4", 48, 48, 23, 38, 384, 0),
    ("enemy1", 101, 101, 51, 97, 577, 0)
;

INSERT INTO Sprite (entity, name, x, y)
VALUES
    ("test", "hitbox-bg", 64, 128),
    -- game
    ("game-ui", "selection-rect", 256, 128),
    ("game-ui", "tile-indicator", 128, 128),
    -- editor
    ("editor-ui", "tilemenu-bg", 0, 128),
    ("editor-ui", "tilemenu-hovering", 64, 128),
    ("editor-ui", "tilemenu-selected", 256, 128),
    -- units
    ("unit4", "selection", 48, 432),
    ("unit4", "dead", 0, 432)
;

INSERT INTO Animation (entity, name, frames, x, y)
VALUES 
    ("unit4", "idle-right", 1, 0, 48),
    ("unit4", "idle-up", 1, 0, 144),
    ("unit4", "idle-left", 1, 0, 240),
    ("unit4", "idle-down", 1, 0, 336),
    -- move
    ("unit4", "move-right", 4, 0, 48),
    ("unit4", "move-up", 4, 0, 144),
    ("unit4", "move-left", 4, 0, 240),
    ("unit4", "move-down", 4, 0, 336),
    -- die
    ("unit4", "die", 4, 0, 384),

    ("enemy1", "idle-down", 1, 0, 0),
    ("enemy1", "idle-up", 1, 0, 144),
    ("enemy1", "idle-right", 1, 0, 48),
    ("enemy1", "idle-left", 1, 0, 240),
    -- move
    ("enemy1", "move-down", 8, 0, 404),
    ("enemy1", "move-up", 8, 0, 505),
    ("enemy1", "move-left", 8, 0, 606),
    ("enemy1", "move-right", 8, 0, 707)
;

INSERT INTO Hitbox (entity, offset_x, offset_y, w, h)
VALUES
    ("unit4", 12, 28, 22, 29),
    ("enemy1", 19, 82, 35, 87)
;

INSERT INTO Tile_Region (name, sprite_origin_x, sprite_origin_y, sprite_w, 
    sprite_h, columns, rows)
VALUES 
    ("test_tiles", 0, 256, 128, 128, 2, 4)
;
