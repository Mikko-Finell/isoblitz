#ifndef animation_hpp
#define animation_hpp

#include "sprite.hpp"
#include <vector>
#include <unordered_map>
#include <string>

namespace impl {
class Sequence {
    std::vector<sf::IntRect> frames;
    time_t frame_duration = 1000.0 / 8.0;
    time_t current_dt = 0;
    int frame = 0;

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

public:
    Sprite sprite;

    Animation() {}
    Animation(const std::string & n);

    void init(RenderSystem & rs);
    void update(time_t dt);
    void add_sequence(const std::string & name, const impl::Sequence & sq);
    void set_sequence(const std::string & name);
};

#endif
