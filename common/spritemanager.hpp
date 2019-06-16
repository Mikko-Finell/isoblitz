#ifndef spritemanager_hpp
#define spritemanager_hpp

#include "sprite.hpp"
#include <list>

class SpriteManager {
    std::list<Sprite> sprites;

public:
    ~SpriteManager();
    Sprite * alloc();
    void destroy(Sprite * sprite);
};

#endif
