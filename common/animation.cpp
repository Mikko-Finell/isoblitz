#include "animation.hpp"
#include "util.hpp"
#include <cassert>
#include <iostream>

namespace impl {
Sequence::Sequence(sf::IntRect rect, int framecount, int padding) {
    for (int i = 0; i < framecount; i++) {
        frames.emplace_back(rect.left, rect.top, rect.width, rect.height);
        rect.left += rect.width + padding;
    }
    assert(framecount > 0);
}

void Sequence::init(Sprite & sprite) {
    sprite.set_spritecoords(frames[frame]);
}

void Sequence::update(time_t dt, Sprite & sprite) {
    assert(frames.size() > 0);

    current_dt += dt;
    if (current_dt >= frame_duration) {
        current_dt = 0;
        frame++;
        frame = frame % frames.size();
        sprite.set_spritecoords(frames[frame]);
    }
}

// TODO easy
// consider whether this should also change the spritecoords
void Sequence::reset() {
    current_dt = 0;
    frame = 0;
}
} // impl

Animation::~Animation() {
    if (anims != nullptr) {
        anims->remove(this);
    }
}

Animation::Animation(const std::string & n) {
    name(n);
}

Animation::Animation(const Animation & other) {
    operator=(other);
}

Animation & Animation::operator=(const Animation & other) {
    throw std::logic_error{"Not implemented."};
    //name = other.name;
    //sequences = other.sequences;
    //current_sequence = other.current_sequence;
    // TODO critical
    // This is merely copying a pointer, meaning that multiple animations
    // can point to same sprite.
    //sprite = other.sprite;
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
    assert(sprite != nullptr);
    sequences.at(_current_sequence).update(dt, *sprite);
}

void Animation::copy_sequences(const Animation & other) {
    name(other.name());
    sequences = other.sequences;
    _current_sequence = other.current_sequence();
}

void Animation::add_sequence(const std::string & sq_name, const impl::Sequence & sq)
{
    sequences[sq_name] = sq;
    if (_current_sequence.empty()) {
        set_sequence(sq_name, "add_sequence");
    }
}

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
    _current_sequence = sq_name;
    if (sprite != nullptr) {
        sequence->init(*sprite);
    }
}

// AnimationSystem //////////////////////////////////////////////////////////////

void AnimationSystem::add(Animation * anim) {
    assert(animations.insert(anim).second == true);
    anim->anims = this;
}

void AnimationSystem::remove(Animation * anim) {
    // erase(key_type) returns the number of elements removed, so we assure
    // that we are not trying to erase non-existant animations. 
    assert(animations.erase(anim) == 1);
    anim->anims = nullptr;
}
void AnimationSystem::update(time_t dt) {
    for (auto anim : animations) {
        anim->update(dt);
    }
}
