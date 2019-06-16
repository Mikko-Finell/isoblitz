#ifndef spritefactory_hpp
#define spritefactory_hpp

#include "sprite.hpp"
#include "spritemanager.hpp"
#include <string>
#include <unordered_map>
#include <memory>

/**
 * SpriteFactory
 * Knows about sprites by name.
 */
class SpriteFactory {
    std::unordered_map<std::string, std::unordered_map<std::string, Sprite>> sprites;
    SpriteManager & spritem;

public:
    SpriteFactory(SpriteManager & sm);
    Sprite * create(RenderSystem & rs, const std::string & entity, const std::string & sprite) const;
    Sprite create(const std::string & entity, const std::string & name) const;
    Sprite * copy(RenderSystem & rs, const Sprite & source) const;
};

#endif
