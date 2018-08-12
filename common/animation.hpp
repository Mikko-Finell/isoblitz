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
    Frame(const sf::IntRect & rect);
    sf::IntRect get_rect() const;
    sf::Vector2i get_coords() const;
    sf::Vector2i get_size() const;

    std::string info() const {
        std::string str;
        str += "Sprite{";
        str += std::to_string(spritecoords.left) + ','
            + std::to_string(spritecoords.top) + '/'
            + std::to_string(spritecoords.width) + ','
            + std::to_string(spritecoords.height) + '}';
        return str;
    }
};

class Sequence {
    std::vector<Frame> frames;
    time_t frame_duration = 1000.0 / 8.0;
    time_t current_dt = 0;
    int frame = 0;
    std::string next;

public:
    Sequence() {}
    Sequence(int x, int y, int w, int h, int framecount, int padding);

    void init(Sprite & sprite);
    void update(time_t dt, Sprite & sprite);
    void reset();

    std::string info() const {
        std::string str;
        str += "Frame=" + std::to_string(frame + 1) + '/'
            + std::to_string(frames.size()) + ", ";
        str += frames[frame].info();
        return str;
    }
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

    void __init(SpriteManager & spritem);
    void update(time_t dt);
    void add_sequence(const std::string & name, const impl::Sequence & sq);
    void set_sequence(const std::string & name);

    std::string info() const {
        std::string str;
        for (auto & sequence : sequences) {
            str += name + '-' + sequence.first + ": ";
            str += sequence.second.info() + '\n';
        }
        return str;
    }
};

class AnimationManager {
    std::unordered_map<std::string, Animation> animations;
    SpriteManager & spritem;

public:
    AnimationManager(SpriteManager & sm);
    Animation get(const std::string & name);

    std::string info() const {
        std::string str{"Animations: \n"};
        for (auto & animation : animations) {
            str += animation.first + ":\n" + animation.second.info()
                + "...............";
        }
        return str;
    }
};

} // gfx

#endif
