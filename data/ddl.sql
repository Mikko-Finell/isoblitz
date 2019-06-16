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

    PRIMARY KEY (entity, name),
    FOREIGN KEY (entity) REFERENCES Entity (name)
);

CREATE TABLE Animation (
    name varchar(255) NOT NULL,
    sequence varchar(255) NOT NULL,
    
    frames INTEGER NOT NULL,
    
    -- pixels between each frame in the sequence
    pad INTEGER DEFAULT 0,

    PRIMARY KEY (name, sequence),
    FOREIGN KEY (name, sequence) REFERENCES Sprite (entity, name)
);

CREATE TABLE Tile_Region (
    name varchar(255) PRIMARY KEY,

    -- size of the region
    columns INTEGER NOT NULL,
    rows INTEGER NOT NULL,

    FOREIGN KEY (name) REFERENCES Entity (name)
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
