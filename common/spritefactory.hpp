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
    std::unordered_map<std::string, std::unordered_map<std::string, SpriteImpl>> sprites;
    SpriteManager & spritem;

public:
    SpriteFactory(SpriteManager & sm);
    Sprite copy(RenderSystem & rs, const Sprite & source);
    SpriteImpl * copy(RenderSystem & rs, const SpriteImpl * source);
    SpriteImpl create(const std::string & entity, const std::string & name);
    Sprite create(RenderSystem & rs, const std::string & entity, const std::string & sprite);

    Sprite create_from_impl(RenderSystem & rs, const SpriteImpl * source);
};

#endif
