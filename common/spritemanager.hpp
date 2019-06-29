#ifndef spritemanager_hpp
#define spritemanager_hpp

#include "sprite.hpp"
#include "stl.hpp"

class SpriteManager {
    std::list<SpriteImpl> sprites;

public:
    ~SpriteManager();
    SpriteImpl * alloc();
    void destroy(SpriteImpl * sprite);
};

#endif
