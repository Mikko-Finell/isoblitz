#ifndef entitycursor_hpp
#define entitycursor_hpp

#include "common/tile.hpp"
#include "common/engine.hpp"

class EntityCursor {
    Entity cursor;
    Engine & engine;

public:
    ~EntityCursor();
    EntityCursor(Engine & engine);
    void set_entity_type(const Entity::Type & type);
    void update_mousepos(const Position & pos);
    Entity::Type get_type() const;
    Tile::Coord get_coordinate() const;
};

#endif
