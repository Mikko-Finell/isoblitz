CREATE TABLE Directions (
    dir varchar(255),
    ordering INTEGER
);

INSERT INTO Directions (dir, ordering) VALUES
    ("down", 0),
    ("down-left", 1),
    ("left", 2),
    ("up-left", 3),
    ("up", 4),
    ("up-right", 5),
    ("right", 6),
    ("down-right", 7)
;

CREATE TABLE Actions (
    type varchar(255)
);

INSERT INTO Actions (type) VALUES
    ("idle"), ("move"), ("fight"), ("die"), ("construct"), ("misc")
;

CREATE TABLE Entity (
    type varchar(255) NOT NULL,
    action_type varchar(255) NOT NULL,
    directions varchar(255),
    PRIMARY KEY (type, action_type, directions)
    FOREIGN KEY (action_type) REFERENCES Actions (type)
);

INSERT INTO Entity (type, action_type, directions) VALUES
    ("unit4", "idle", "all"),
    ("unit4", "move", "all"),
    ("unit4", "die", NULL),
    ("soldier", "idle", "all"),
    ("soldier", "move", "all"),
    ("soldier", "fight", "all"),
    ("soldier2", "idle", "all"),
    ("soldier2", "move", "all"),
    ("electrocannon", "idle", NULL),
    ("electrocannon", "construct", NULL),
    ("electrocannon", "fight", NULL),
    ("electrocannon", "die", NULL)
;

CREATE TABLE Sprite (
    name varchar(255) PRIMARY KEY,
    -- the coords of the sprite
    x INTEGER DEFAULT 0,
    y INTEGER DEFAULT 0,
    -- sprite size
    w INTEGER DEFAULT 128,
    h INTEGER DEFAULT 128,
    -- used for sprite alignment
    offset_x INTEGER DEFAULT 0, -- distance from cell coordinate
    offset_y INTEGER DEFAULT 0
);

INSERT INTO Sprite (name, x, y, w, h, offset_x, offset_y) VALUES 
    ("test", 0, 0, 32, 32, 0, 0),
    ("debug1", 0, 0, 16, 16, 0, 0),
    ("minimap-world", 0, 0, 256, 256, 0, 0),
    -- entities
    ("unit4", 384, 0, 48, 48, 23, 38),
    -- units
    -- TODO maybe a seperate table for sprites belonging to an entity
    ("unit4-selection", 432, 432, 48, 48, 23, 38),
    ("unit4-dead", 384, 432, 48, 48, 23, 38),
    ("soldier-selection", 432, 432, 48, 48, 23, 38),
    ("soldier2-selection", 432, 432, 48, 48, 23, 38),

    ("enemy1", 577, 0, 100, 100, 51, 97),
    ("soldier", 2058, 0, 95, 95, 48, 78),
    ("soldier2", 2297, 0, 76, 76, 38, 63),
    ("electrocannon", 1390, 1085, 48, 48, 25, 29),
    -- tiles
    ("test_tiles", 0, 256, 128, 128, 0, 0)
;

INSERT INTO Sprite (name, x, y, w, h) VALUES
    ("hitbox-bg", 288, 128, 32, 32),
    ("pathcell", 272, 160, 16, 16),
    -- game
    ("selection-rect", 256, 128, 32, 32),
    ("tile-indicator", 128, 128, 32, 32),
    -- editor
    ("tilemenu-bg", 256, 96, 32, 32),
    ("tilemenu-hovering", 288, 96, 32, 32),
    ("tilemenu-selected", 320, 96, 32, 32)
;

CREATE TABLE AnimationMeta (
    entity_type varchar(255), -- unit4
    action_type varchar(255), -- move
    sequence_name varchar(255) DEFAULT "DEFAULT", -- down-left
    frames INTEGER DEFAULT 1,
    pad INTEGER DEFAULT 0,
    loop BOOLEAN DEFAULT 0,
    region_x INTEGER,
    region_y INTEGER,
    -- TODO possibly remove these, in case we want non-entity animations
    FOREIGN KEY (entity_type) REFERENCES Entity (type),
    FOREIGN KEY (action_type) REFERENCES Actions (type)
);

INSERT INTO AnimationMeta (entity_type, action_type, frames, pad, loop, region_x, region_y) VALUES
    ("unit4", "idle", 1, 0, 0, 384, 0),
    ("unit4", "move", 4, 0, 1, 384, 0),
    ("unit4", "die", 4, 0, 0, 384, 384),
    ("soldier", "idle", 1, 1, 0, 2058, 0),
    ("soldier", "move", 4, 1, 1, 1674, 0),
    ("soldier", "fight", 3, 1, 0, 1386, 0),
    ("soldier2", "idle", 1, 1, 1, 2297, 0),
    ("soldier2", "move", 12, 1, 1, 2372, 0),
    ("electrocannon", "idle", 1, 1, 0, 1390, 1085),
    ("electrocannon", "construct", 8, 1, 0, 1390, 1379),
    ("electrocannon", "fight", 5, 1, 1, 1390, 1281),
    ("electrocannon", "die", 8, 1, 0, 1390, 1232)
;

CREATE TABLE Animation (
    entity_type varchar(255), -- unit4
    action_type varchar(255),
    sequence_name varchar(255),
    frames INTEGER DEFAULT 1,
    pad INTEGER DEFAULT 0,
    loop BOOLEAN DEFAULT 0,
    region_x INTEGER, -- TODO rename to sequence_x
    region_y INTEGER,
    FOREIGN KEY (action_type) REFERENCES Actions (type),
    FOREIGN KEY (sequence_name) REFERENCES Directions (dir)
);

INSERT INTO Animation (entity_type, action_type, sequence_name)
    SELECT DISTINCT Entity.type, entity.action_type,
    CASE WHEN Entity.directions="all"
         THEN Directions.dir
         ELSE "down"
    END
    FROM Entity, Directions
;

UPDATE Animation SET 
    frames = (SELECT AnimationMeta.frames FROM AnimationMeta
        WHERE Animation.action_type = AnimationMeta.action_type
        AND Animation.entity_type = AnimationMeta.entity_type),
    pad = (SELECT AnimationMeta.pad FROM AnimationMeta
        WHERE Animation.action_type = AnimationMeta.action_type
        AND Animation.entity_type = AnimationMeta.entity_type),
    loop = (SELECT AnimationMeta.loop FROM AnimationMeta
        WHERE Animation.action_type = AnimationMeta.action_type
        AND Animation.entity_type = AnimationMeta.entity_type),
    region_x = (SELECT AnimationMeta.region_x FROM AnimationMeta
        WHERE Animation.action_type = AnimationMeta.action_type
        AND Animation.entity_type = AnimationMeta.entity_type),
    region_y = (SELECT AnimationMeta.region_y FROM AnimationMeta
        WHERE Animation.action_type = AnimationMeta.action_type
        AND Animation.entity_type = AnimationMeta.entity_type)
;

CREATE TEMPORARY TABLE _tmp AS
    SELECT Animation.entity_type, Animation.action_type, Animation.sequence_name, 
        region_y + ordering * (h + pad)
        AS new_region_y
    FROM Animation
    INNER JOIN Directions ON sequence_name=dir
    INNER JOIN Sprite ON Animation.entity_type=Sprite.name
;

UPDATE Animation
    SET region_y = (SELECT new_region_y
                    FROM _tmp
                    WHERE animation.entity_type=_tmp.entity_type
                    and animation.action_type=_tmp.action_type and
                    animation.sequence_name=_tmp.sequence_name)
    where EXISTS (SELECT new_region_y
                    FROM _tmp
                    WHERE animation.entity_type=_tmp.entity_type
                    and animation.action_type=_tmp.action_type and
                    animation.sequence_name=_tmp.sequence_name)
;

DROP TABLE _tmp;

CREATE TABLE Tile_Region (
    name varchar(255) PRIMARY KEY,
    columns INTEGER NOT NULL,
    rows INTEGER NOT NULL,
    FOREIGN KEY (name) REFERENCES Entity (type)
);

INSERT INTO Tile_Region (name, columns, rows) VALUES 
    ("test_tiles", 2, 4)
;

CREATE TABLE Hitbox (
    entity varchar(255) NOT NULL PRIMARY KEY,
    -- distance from cell coordinate
    offset_x INTEGER DEFAULT 0, 
    offset_y INTEGER DEFAULT 0,
    w INTEGER NOT NULL,
    h INTEGER NOT NULL,

    FOREIGN KEY (entity) REFERENCES Entity (type)
);

INSERT INTO Hitbox (entity, offset_x, offset_y, w, h) VALUES
    ("unit4", 11, 29, 22, 29),
    ("enemy1", 19, 82, 35, 87),
    ("soldier", 0, 0, 0, 0), -- TODO
    ("soldier2", 0, 0, 0, 0), -- TODO
    ("electrocannon", 24, 31, 48, 48)
;
