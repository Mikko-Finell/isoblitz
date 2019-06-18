#ifndef animation_hpp
#define animation_hpp

#include "sprite.hpp"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <memory>

namespace impl {
/**
 * Sequence
 * A series of frames.
 */
class Sequence {
    std::vector<sf::IntRect> frames;
    time_t frame_duration = 1000.0 / 8.0;
    time_t current_dt = 0;
    int frame = 0;

public:
    Sequence() {}
    Sequence(sf::IntRect rect, int frames, int pad);

    void update(time_t dt, Sprite & sprite);

    /* sets the sequence back to initial state */
    void reset(); // note: has no effect on sprite
};
} // impl

/**
 * Animation 
 * Holds named sequences.
 */
class AnimationSystem;
class Animation { 
    std::string _name;
    std::unordered_map<std::string, impl::Sequence> sequences;
    std::string _current_sequence;

    Animation(const Animation & other);
    Animation & operator=(const Animation & other);

public:
    AnimationSystem * anims = nullptr;
    Sprite sprite;

    virtual ~Animation();
    Animation(const std::string & n);

    /* Initialize the animation. 
     * Calling other methods before this is undefined behavior */
    void init();

    void update(time_t dt);
    void copy_sequences(const Animation & other);
    void add_sequence(const std::string & name, const impl::Sequence & sq);
    void set_sequence(const std::string & name, const std::string & caller);
    const std::string & name() const;
    const std::string & name(const std::string & n);
    std::string current_sequence() const;
};

/**
 * AnimationSystem
 * Responsible for updating all animations.
 */
class AnimationSystem {
    std::unordered_set<Animation *> animations;

public:
    void add(Animation * anim);
    inline void add(Animation & anim) {
        add(&anim);
    }
    void remove(Animation * anim);
    void update(time_t dt);
};

#endif
