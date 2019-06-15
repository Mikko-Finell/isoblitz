#ifndef spritefactory_hpp
#define spritefactory_hpp

#include "sprite.hpp"
#include "spritemanager.hpp"
#include <string>
#include <unordered_map>

/**
 * SpriteFactory
 * Knows about sprites by name.
 */
class SpriteFactory {
    using key_t = std::string;
    std::unordered_map<key_t, std::unordered_map<key_t, Sprite>> sprites;
    SpriteManager & spritem;

public:
    SpriteFactory(SpriteManager & sm);
    Sprite * create(RenderSystem & rs, const std::string & entity, const std::string & sprite);
};

#endif
