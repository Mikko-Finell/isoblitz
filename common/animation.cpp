#include "animation.hpp"
#include "util.hpp"
#include <cassert>
#include <iostream>

namespace impl {
Sequence::Sequence(int x, int y, int w, int h, int framecount, int padding) {
    for (int i = 0; i < framecount; i++) {
        frames.emplace_back(x, y, w, h);
        x += w + padding;
    }
    assert(framecount != 0);
}

void Sequence::init(Sprite & sprite) {
    sprite.set_spritecoords(frames[frame]);
}

void Sequence::update(time_t dt, Sprite & sprite) {
  /*
    assert(frames.size() != 0);

    current_dt += dt;
    if (current_dt >= frame_duration) {
        current_dt = 0;
        frame++;
        frame = frame % frames.size();
        sprite.set_spritecoords(frames[frame]);
    }
    */
}

// TODO easy
// consider whether this should also change the spritecoords
void Sequence::reset() {
    current_dt = 0;
    frame = 0;
}
} // impl

Animation::~Animation() {
    //unreg();
}

Animation::Animation(const std::string & n) : name(n) {
}

Animation::Animation(const Animation & other) {
    operator=(other);
}

Animation & Animation::operator=(const Animation & other) {
    name = other.name;
    sequences = other.sequences;
    current_sequence = other.current_sequence;
    sprite = other.sprite;

    /*
    if (auto as = dynamic_cast<AnimationSystem *>(other.system); as) {
        // TODO easy
        // add assigns the system, no need to do it here
        system = as;
        as->add(this);
    }
    */
    return *this;
}

void Animation::init() {
    for (auto & pair : sequences) {
        pair.second.reset();
    }
    assert(sequences.empty() == false);
    set_sequence(sequences.begin()->first, "Animation::init");
}

void Animation::update(time_t dt) {
    sequences.at(current_sequence).update(dt, sprite);
}

// TODO easy
// perhaps error if sequence already exists
void 
Animation::add_sequence(const std::string & sq_name, const impl::Sequence & sq)
{
    sequences[sq_name] = sq;
}

// TODO easy
// should current sequence reset when changing?
void Animation::set_sequence(const std::string & sq_name, const std::string & caller) {
    impl::Sequence * sequence = nullptr;
    try {
        sequence = &sequences.at(sq_name);
    }
    catch (std::out_of_range) {
        std::cerr << "\nERROR: Animation::set_sequence(" << sq_name << ")\n";
        std::cerr << " *** Caller: " << caller << "\n";
        //std::terminate();
        throw;
    }
    current_sequence = sq_name;
    sequence->init(sprite);
}

// AnimationSystem //////////////////////////////////////////////////////////////

void AnimationSystem::add(Animation * anim) {
    assert(animations.insert(anim).second == true);
    //anim->reg(this);
}

void AnimationSystem::remove(Animation & anim) {
    // erase(key_type) returns the number of elements removed, so we assure
    // that we are not trying to erase non-existant animations. 
    assert(animations.erase(&anim) == 1);
}

//void AnimationSystem::remove(GameObject * go) {
    //auto anim = dynamic_cast<Animation *>(go);
    //assert(anim != nullptr);

    //animations.erase(anim);
    //go->reg(nullptr);
//}

void AnimationSystem::update(time_t dt) {
    //std::cout << "Updating " << animations.size() << " anims\n";
    for (auto anim : animations) {
        //std::cout << " * Updating: " << anim << " ... ";
        anim->update(dt);
        //std::cout << " OK\n";
    }
}
