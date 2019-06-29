#ifndef animation_hpp
#define animation_hpp

#include "sprite.hpp"
#include "observer.hpp"
#include "types.hpp"
#include "stl.hpp"

class AnimationFactory;
class TexCoordSequence {
    friend AnimationFactory;

    std::string name;
    std::vector<sf::IntRect> frames;
    bool loop = false;

    void set_frame(int frame);
    void set_looping(bool on);
    void set_name(const std::string & name);

    //TexCoordSequence() {}
    TexCoordSequence(const std::string & n) :name(n) {}
    TexCoordSequence(sf::IntRect rect, int frames, int pad);

public:
    int advance(int frame) const;
    const std::string & get_name() const;
    const sf::IntRect & get_texcoords(int frame) const;
    const bool is_looping() const;
    const bool has_ended(int frame) const;
    std::string info() const;
};

/**
 * Animation 
 * Holds named sequences.
 */
class ActionToDirectionMap;
class AnimationSystem;
class Animation { 
public:
    Sprite sprite;

    virtual ~Animation();
    Animation(const std::string & n);
    Animation(const std::string & n, AnimationSystem * as);
    Animation(const Animation & other);
    Animation & operator=(const Animation & other);
    Animation & operator=(Animation && other);

    Animation & init();
    Animation & reset();
    Animation & clear();
    Animation & play();
    Animation & play_action(const actions::Type & action);
    Animation & set_direction(const directions::Type & dir);
    Animation & pause();
    void update(float dt);

    void copy_sequences(const Animation & other);
    void set_name(const std::string & n);
    const std::string & get_name() const;
    std::string info() const;

private:
    friend AnimationFactory;
    friend AnimationSystem;
    AnimationSystem * anims = nullptr;
    AnimationFactory * animf = nullptr;
    static constexpr float frame_duration = 1000.0 / config::fps;
    float current_dt = 0;
    int current_frame = 0;
    bool playing = false;
    std::string name;
    actions::Type action;
    directions::Type direction;
    TexCoordSequence * current_sequence = nullptr;
    ActionToDirectionMap * direction_to_sequence = nullptr;
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
