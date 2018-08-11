#ifndef __ANIM__
#define __ANIM__

#include "sprite.hpp"
#include <vector>
#include <unordered_map>
#include <string>

namespace gfx {

using time_t = double;

namespace impl {
class Frame {
    sf::IntRect spritecoords{256, 0, 128, 128};

public:
    sf::IntRect get_rect() const;
    sf::Vector2i get_coords() const;
};

class Sequence {
    std::vector<Frame> frames;
    time_t frame_duration = 1.0;
    int frame = 0;
    std::string next;
    std::string prev;

public:
    void update(time_t dt);
    void set_loop(bool b);
    void set_next(const std::string & name);
    std::string next_sequence() const;
    bool finished() const;
    const Frame & current_frame() const;
};
} // impl

class Animation : public Sprite {
    std::unordered_map<std::string, impl::Sequence> sequences;
    std::string current_sequence;
    impl::Sequence * active_sequence = nullptr;

public:
    using Sprite::Sprite;
    void update(time_t dt);
    void add_sequence(const std::string & name, const impl::Sequence & s);
    void set_sequence(const std::string & name);
};

class AnimationManager {
    std::unordered_map<std::string, impl::Sequence> sequences;
    SpriteManager & spritem;

public:
    AnimationManager(SpriteManager & sm);
};

} // gfx

#endif
