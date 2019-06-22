#include "animation.hpp"
#include "util.hpp"
#include "entity.hpp"
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
    reset();
    sprite.set_texcoords(frames[frame]);
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

void Sequence::reset() {
    current_dt = frame_duration;
    frame = -1;
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
    if (sequences.count(sq_name) == 0) {
        throw std::out_of_range{"Animation::set_sequence(" + sq_name + ")"};
    }
    else if (_current_sequence == sq_name) {
        return;
    }
    else {
        _current_sequence = sq_name;
        sequences[sq_name].reset();
    }
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

void AnimationSystem::on_entity_moved(Entity & entity, const sf::Vector2f & vector) {
    // copypasted unchecked code from stackexchange
    enum compassDir {
        left = 0, up_left = 1, up = 2, up_right = 3, 
        right = 4, down_right = 5, down = 6, down_left = 7
    };
    static const std::string vec_to_dir[8] = {
        "down", "down-left", "left", "up-left", 
        "up", "up-right", "right", "down-right"
    };
    // actual conversion code:
    float angle = atan2( vector.y, vector.x );
    // add 45 degrees because of isometric rotation
    angle -= M_PI/4;
    int octant = int( 8 * angle / (2*M_PI) + 8.5 ) % 8;
    compassDir dir = (compassDir) octant;  // typecast to enum: 0 -> E etc.

    if (vector.x == 0 and vector.y == 0) {
        auto sq = entity.animation.current_sequence();
        entity.animation.set_sequence("idle-" + sq.substr(5));
    }
    else {
        auto str = "move-" + vec_to_dir[octant];
        entity.animation.set_sequence(str);
    }
}
