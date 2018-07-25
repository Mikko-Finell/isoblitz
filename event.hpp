#ifndef __EVENT__
#define __EVENT__

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
	DeleteTile,
	Undo,
    };
    
    Type type = None;
    void * data;
    void * sender;
    Event(Type type = None, void * d = nullptr, void * s = nullptr);
    bool operator==(Type type) const;
};
    
#endif
