#ifndef animation_hpp
#define animation_hpp

#include "sprite.hpp"
#include "observer.hpp"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <memory>

class TexCoordSequence {
    std::vector<sf::IntRect> frames;
    int current_frame = 0;
    bool loop = false;
    std::string name;
public:
    TexCoordSequence() {}
    TexCoordSequence(const std::string & n) :name(n) {}
    TexCoordSequence(sf::IntRect rect, int frames, int pad);
    void advance();
    void set_frame(int frame);
    void set_looping(bool on);
    void reset();
    void set_name(const std::string & name);
    const std::string & get_name() const;
    const sf::IntRect & get_texcoords() const;
    const int get_frame() const;
    const bool is_looping() const;
    const bool has_ended() const;
};

/**
 * Animation 
 * Holds named sequences.
 */
class AnimationSystem;
class AnimationFactory;
class Animation { 
    friend AnimationSystem;
    AnimationSystem * anims = nullptr;
    AnimationFactory * animf = nullptr;
    float frame_duration = 1000.0 / config::fps;
    float current_dt = 0;

    std::string name;
    std::unordered_map<std::string, TexCoordSequence> sequences;
    std::string current_sequence_name;

public:
    Sprite sprite;

    virtual ~Animation();
    Animation(const std::string & n);
    Animation(const std::string & n, AnimationSystem * as);
    Animation(const Animation & other);
    Animation & operator=(const Animation & other);
    Animation & operator=(Animation && other);

    /* Initialize the animation. 
     * Calling other methods before this is undefined behavior */
    void init();
    void clear();

    void update(float dt);
    void copy_sequences(const Animation & other);
    void add_sequence(const TexCoordSequence & sequence);
    void set_sequence(const std::string & name);
    TexCoordSequence & get_current_sequence();
    const TexCoordSequence & get_current_sequence() const;
    void set_dt(float dt);
    const float get_dt() const;
    void set_name(const std::string & n);
    const std::string & get_name() const;
};

/**
 * AnimationSystem
 * Responsible for updating all animations.
 */
class Entity;
class AnimationSystem : public Observer {
    std::unordered_set<Animation *> animations;

public:
    void add(Animation * anim);
    inline void add(Animation & anim) {
        add(&anim);
    }
    void remove(Animation * anim);
    void update(time_t dt);
    void on_entity_moved(Entity & entity, const sf::Vector2f & vector);
};

#endif
