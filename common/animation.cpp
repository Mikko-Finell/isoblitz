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

void Sequence::update(time_t dt, Sprite & sprite) {
    assert(frames.size() > 0);

    current_dt += dt;
    if (current_dt >= frame_duration) {
        current_dt = 0;
        frame++;
        frame = frame % frames.size();
        sprite.set_texcoords(frames[frame]);
    }
}

// TODO easy
// consider whether this should also change the spritecoords
void Sequence::reset() {
    current_dt = 0;
    frame = 0;
}
} // impl

void Animation::clear() {
    if (anims != nullptr) {
        anims->remove(this);
    }
    anims = nullptr;
    sprite.clear();
}

Animation::~Animation() {
    clear();
}

Animation::Animation(const std::string & n) {
    name(n);
}

Animation::Animation(const std::string & n, AnimationSystem * as)
    : _name(n)
{
    as->add(this);
}

Animation::Animation(const Animation & other) {
    operator=(other);
}

Animation & Animation::operator=(const Animation & other) {
    if (other.sequences.empty() == false) {
        copy_sequences(other);
        set_sequence(other.current_sequence());
    }
    name(other.name());
    sprite = other.sprite;
    if (anims == nullptr && other.anims != nullptr) {
        other.anims->add(this);
    }
    return *this;
}

Animation & Animation::operator=(Animation && other) {
    operator=(other);
    other.clear();
    return *this;
}

void Animation::init() {
    for (auto & pair : sequences) {
        pair.second.reset();
    }
    set_sequence(current_sequence());
}

void Animation::update(time_t dt) {
    sequences.at(_current_sequence).update(dt, sprite);
}

void Animation::copy_sequences(const Animation & other) {
    name(other.name());
    sequences.clear();
    sequences = other.sequences;
    _current_sequence = other.current_sequence();
}

void Animation::add_sequence(const std::string & sq_name, const impl::Sequence & sq)
{
    assert(sq_name.empty() == false);
    sequences[sq_name] = sq;
    if (_current_sequence.empty()) {
        set_sequence(sq_name);
    }
}

void Animation::set_sequence(const std::string & sq_name) {
    try {
        auto s = sequences.at(sq_name);
    }
    catch (std::out_of_range) {
        std::cerr << "\nERROR: Animation::set_sequence(" << sq_name << ")\n";
        throw;
    }
    _current_sequence = sq_name;
}

const std::string & Animation::name() const {
    return _name;
}

const std::string & Animation::name(const std::string & n) {
    _name = n;
    return name();
}

std::string Animation::current_sequence() const {
    return _current_sequence;
}

// AnimationSystem //////////////////////////////////////////////////////////////

void AnimationSystem::add(Animation * anim) {
    assert(anim->anims == nullptr);
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
