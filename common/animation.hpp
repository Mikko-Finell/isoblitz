#ifndef animation_hpp
#define animation_hpp

#include "sprite.hpp"
#include "system.hpp"
#include <vector>
#include <unordered_map>
#include <string>

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
    Sequence(int x, int y, int w, int h, int frames, int pad);

    /* initialize the sequence, calling other methods before this
     * is undefined behavior */
    void init(Sprite & sprite);

    void update(time_t dt, Sprite & sprite);

    /* sets the sequence back to initial state */
    void reset(); // note: has no effect on sprite
};
} // impl

/**
 * Animation 
 * Holds named sequences.
 */
class Animation : public GameObject {
    std::string name;
    std::unordered_map<std::string, impl::Sequence> sequences;
    std::string current_sequence = "DEFAULT SEQUENCE";

public:
    Sprite sprite;

    virtual ~Animation();

    Animation(const std::string & n);
    Animation(const Animation & other);
    Animation & operator=(const Animation & other);

    /* Initialize the animation. 
     * Calling other methods before this is undefined behavior */
    void init();

    void update(time_t dt);
    void add_sequence(const std::string & name, const impl::Sequence & sq);
    void set_sequence(const std::string & name);
    std::string get_current_sequence() const {
        return current_sequence;
    }
};

/**
 * AnimationSystem
 * Responsible for updating all animations.
 */
class AnimationSystem : public System {
    std::unordered_set<Animation *> animations;

public:
    void add(Animation * anim);
    inline void add(Animation & anim) {
        add(&anim);
    }
    void remove(GameObject * go) override;
    inline void remove(Animation & anim) {
        remove(&anim);
    }
    void update(time_t dt);
};

#endif
