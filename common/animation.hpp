#ifndef __ANIM__
#define __ANIM__

#include "sprite.hpp"
#include <vector>
#include <unordered_map>
#include <string>

namespace gfx {

using time_t = double;

namespace impl {
class Sequence {
    std::vector<sf::IntRect> frames;
    time_t frame_duration = 1000.0 / 8.0;
    time_t current_dt = 0;
    int frame = 0;
    std::string next;

public:
    Sequence() {}
    Sequence(int x, int y, int w, int h, int frames, int pad);

    void init(Sprite & sprite);
    void update(time_t dt, Sprite & sprite);
    void reset();
};
} // impl

class Animation {
    std::string name;
    std::unordered_map<std::string, impl::Sequence> sequences;
    std::string current_sequence;
    impl::Sequence * active_sequence = nullptr;
    int offset_x = 0, offset_y = 0;

public:
    Sprite sprite;

    Animation() {}
    Animation(const std::string & n);

    void __init(SpriteManager & spritem);
    void __set_offset(int x, int y);
    void update(time_t dt);
    void add_sequence(const std::string & name, const impl::Sequence & sq);
    void set_sequence(const std::string & name);
};

class AnimationManager {
    std::unordered_map<std::string, Animation> animations;
    SpriteManager & spritem;

public:
    AnimationManager(SpriteManager & sm);
    Animation get(const std::string & name);
};

} // gfx

#endif
