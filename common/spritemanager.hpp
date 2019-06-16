#ifndef spritemanager_hpp
#define spritemanager_hpp

#include "sprite.hpp"
#include <list>

class SpriteManager {
    std::list<Sprite> sprites;

public:
    ~SpriteManager();
    Sprite * alloc();
    void destroy(Sprite & sprite);
    inline void destroy(Sprite * sprite) {
        destroy(*sprite);
    }
};

#endif
