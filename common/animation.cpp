#include "animation.hpp"
#include "util.hpp"
#include "entity.hpp"
#include <cassert>
#include <iostream>

TexCoordSequence::TexCoordSequence(sf::IntRect rect, int framecount, int padding) {
    for (int i = 0; i < framecount; i++) {
        frames.emplace_back(rect.left, rect.top, rect.width, rect.height);
        rect.left += rect.width + padding;
    }
    assert(framecount > 0);
}

void TexCoordSequence::advance() {
    assert(frames.size() > 0);
    if (is_looping()) {
        current_frame = (current_frame + 1) % frames.size();
    }
    else {
        if (current_frame < frames.size()) {
            ++current_frame;
        }
    }
}

void TexCoordSequence::reset() {
    current_frame = 0;
}

void TexCoordSequence::set_name(const std::string & n) {
    name = n;
}

const std::string & TexCoordSequence::get_name() const {
    return name;
}

void TexCoordSequence::set_frame(int f) {
    current_frame = f % frames.size();
}

void TexCoordSequence::set_looping(bool on) {
    loop = on;
}

const sf::IntRect & TexCoordSequence::get_texcoords() const {
    if (has_ended()) {
        return frames[frames.size() - 1];
    }
    else {
        return frames[current_frame];
    }
}

const int TexCoordSequence::get_frame() const {
    return current_frame;
}

const bool TexCoordSequence::is_looping() const {
    return loop;
}

const bool TexCoordSequence::has_ended() const {
    return is_looping() == false and current_frame == frames.size();
}

/////////////////////////////////////////////////////////////////////// ANIMATION

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

Animation::Animation(const std::string & n) :name(n) {
}

Animation::Animation(const std::string & n, AnimationSystem * as)
    : name(n)
{
    as->add(this);
}

Animation::Animation(const Animation & other) {
    operator=(other);
}

Animation & Animation::operator=(const Animation & other) {
    sprite = other.sprite;
    if (other.sequences.empty() == false) {
        copy_sequences(other);
        set_sequence(other.current_sequence().get_name());
    }
    set_name(other.get_name());
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
    set_sequence(current_sequence().get_name());
}

void Animation::update(float dt) {
    current_dt += dt;
    TexCoordSequence & sequence = current_sequence();
    if (current_dt >= frame_duration) {
        current_dt = 0;
        sequence.advance();
        sprite.set_texcoords(sequence.get_texcoords());
    }
}

void Animation::copy_sequences(const Animation & other) {
    set_name(other.get_name());
    sequences.clear();
    for (auto & pair : other.sequences) {
        add_sequence(pair.second);
    }
}

void Animation::add_sequence(const TexCoordSequence & sequence) {
    assert(sequence.get_name().empty() == false);
    sequences[sequence.get_name()] = sequence;

    if (current_sequence_name.empty()) {
        current_sequence_name = sequence.get_name();
    }
}

void Animation::set_sequence(const std::string & sequence_name) {
    if (sequences.count(sequence_name) == 0) {
        throw std::out_of_range{"Animation::set_sequence(" + sequence_name + ")"};
    }
    auto & sequence = current_sequence();

    sequence.reset();
    current_dt = 0.0f;
    current_sequence_name = sequence_name;
}

void Animation::set_sequence_immediate(const std::string & sequence_name) {
    if (sequences.count(sequence_name) == 0) {
        throw std::out_of_range{"Animation::set_sequence_immediate(" + sequence_name + ")"};
    }
    auto frame = current_sequence().get_frame();
    current_sequence_name = sequence_name;
    current_sequence().set_frame(frame);
    sprite.set_texcoords(current_sequence().get_texcoords());
}

const std::string & Animation::get_name() const {
    return name;
}

void Animation::set_name(const std::string & n) {
    name = n;
}

TexCoordSequence & Animation::current_sequence() {
    return sequences.at(current_sequence_name);
}

const TexCoordSequence & Animation::current_sequence() const {
    return sequences.at(current_sequence_name);
}

void Animation::set_dt(float dt) {
    current_dt = dt;
}

const float Animation::get_dt() const {
    return current_dt;
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
