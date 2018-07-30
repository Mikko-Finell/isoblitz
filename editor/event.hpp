#ifndef __EVENT__
#define __EVENT__

#include <string>
#include <variant>
#include "common/coordinate.hpp"
#include "common/position.hpp"
#include "tile.hpp"

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
	SetSprite,
	ReloadSprites,
	Save,
	Load,
        New,
        SetMapName,
        SetSpriteSheet,
        SetBlocked,
    };

    std::variant<
            bool,
            std::string,
            Coordinate,
            Position,
            Tile> 
        param;
    
    Type type = None;
    void * data;
    void * sender;
    Event(Type type = None, void * d = nullptr, void * s = nullptr);
    bool operator==(Type type) const;

    virtual ~Event() {}
};
    
#endif
