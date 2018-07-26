#ifndef __EVENT__
#define __EVENT__

#include "tile.hpp"
#include "coordinate.hpp"
#include "position.hpp"

class Event {
public:
    enum Type {
	None,
	Test,
	Quit,
	Restart,
	Start,
	Error,
	Scroll,
	CreateTile,
	RemoveTile,
	Undo,
	RegisteredListener,
	Paint,
	Erase,
	MousePosition,
	SetTileType,
	ReloadSprites,
    };
    union {
	void * pointer;
	bool boolean;
	Coordinate coordinate;
	Position position;
	//Tile::Type tile_type;
    };
    
    Type type = None;
    void * data;
    void * sender;
    Event(Type type = None, void * d = nullptr, void * s = nullptr);
    bool operator==(Type type) const;
};
    
#endif
