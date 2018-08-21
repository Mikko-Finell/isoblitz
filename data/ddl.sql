CREATE TABLE Entity (
    name varchar(255) NOT NULL PRIMARY KEY,

    -- sprite size
    sprite_w INTEGER DEFAULT 128,
    sprite_h INTEGER DEFAULT 128,

    -- used for sprite alignment
    sprite_offset_x INTEGER DEFAULT 0, -- distance from cell coordinate
    sprite_offset_y INTEGER DEFAULT 0,
    
    tileset_origin_x INTEGER,
    tileset_origin_y INTEGER
);

CREATE TABLE Sprite (
    entity varchar(255) NOT NULL,
    name varchar(255) NOT NULL,

    x INTEGER DEFAULT 0,
    y INTEGER DEFAULT 0,
    --w INTEGER DEFAULT 128,
    --h INTEGER DEFAULT 128,

    PRIMARY KEY (name, entity),
    FOREIGN KEY (entity) REFERENCES Entity (name)
);

CREATE TABLE Animation (
    entity varchar(255) NOT NULL,
    name varchar(255) NOT NULL,
    
    frames INTEGER NOT NULL,

    x INTEGER DEFAULT 0,
    y INTEGER DEFAULT 0,
    --w INTEGER DEFAULT 128,
    --h INTEGER DEFAULT 128,
    
    -- pixels between each frame in the sequence
    pad INTEGER DEFAULT 0,

    PRIMARY KEY (name, entity),
    FOREIGN KEY (entity) REFERENCES Entity (name)
);

CREATE TABLE Tile_Region (
    name varchar(255) PRIMARY KEY,

    -- spritesheet coord of upper leftmost corner 
    sprite_origin_x INTEGER NOT NULL, 
    sprite_origin_y INTEGER NOT NULL,

    -- sprite size
    sprite_w INTEGER DEFAULT 128,
    sprite_h INTEGER DEFAULT 128,

    -- size of the region
    columns INTEGER,
    rows INTEGER
);

CREATE TABLE Hitbox (
    entity varchar(255) NOT NULL PRIMARY KEY,
    
    -- distance from cell coordinate
    offset_x INTEGER DEFAULT 0, 
    offset_y INTEGER DEFAULT 0,
    w INTEGER NOT NULL,
    h INTEGER NOT NULL,

    FOREIGN KEY (entity) REFERENCES Entity (name)
);
