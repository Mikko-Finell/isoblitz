CREATE TABLE animation_meta (
    fps INTEGER NOT NULL,
    padding INTEGER NOT NULL
);

INSERT INTO animation_meta (fps, padding) VALUES (8, 0);

CREATE TABLE animations (
    name varchar(255) NOT NULL PRIMARY KEY,
    origin_x INTEGER NOT NULL,
    origin_y INTEGER NOT NULL,
    w INTEGER NOT NULL,
    h INTEGER NOT NULL,
    padding INTEGER DEFAULT 0,
    offset_x INTEGER DEFAULT 0,
    offset_y INTEGER DEFAULT 0
);

INSERT INTO animations (name, origin_x, origin_y, w, h, offset_x, offset_y)
VALUES ("test", 384, 0, 48, 48, 23, 38);

CREATE TABLE sequences (
    name varchar(255) NOT NULL,
    animation varchar(255) NOT NULL,
    x INTEGER DEFAULT 256,
    y INTEGER DEFAULT 0,
    frames INTEGER DEFAULT 1,
    PRIMARY KEY (name, animation),
    FOREIGN KEY (animation) REFERENCES animations (name)
);

INSERT INTO sequences (name, animation, x, y, frames)
VALUES 
    ("idle-right", "test", 0, 48, 1),
    ("idle-up", "test", 0, 144, 1),
    ("idle-left", "test", 0, 240, 1),
    ("idle-down", "test", 0, 336, 1),
    -- move
    ("move-right", "test", 0, 48, 4),
    ("move-up", "test", 0, 144, 4),
    ("move-left", "test", 0, 240, 4),
    ("move-down", "test", 0, 336, 4),
    -- die
    ("die", "test", 0, 384, 4)
    ;
