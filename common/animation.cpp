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
    if (other.sequences.empty() == false) {
        copy_sequences(other);
        set_sequence(other.get_current_sequence().get_name());
    }
    set_name(other.get_name());
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
    set_sequence(get_current_sequence().get_name());
}

void Animation::update(float dt) {
    current_dt += dt;
    if (current_dt >= frame_duration) {
        current_dt = 0;
        TexCoordSequence & sequence = get_current_sequence();
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
    //set_sequence(other.get_current_sequence().get_name());
}

void Animation::add_sequence(const TexCoordSequence & sequence) {
    assert(sequence.get_name().empty() == false);
    sequences[sequence.get_name()] = sequence;

    if (current_sequence_name.empty()) {
        set_sequence(sequence.get_name());
    }
}

void Animation::set_sequence(const std::string & sequence_name) {
    if (sequences.count(sequence_name) == 0) {
        throw std::out_of_range{"Animation::set_sequence(" + sequence_name + ")"};
    }
    else if (current_sequence_name == sequence_name) {
        return;
    }
    else {
        current_sequence_name = sequence_name;
        // TODO
        // If sequence is not reset it looks like animaiton is lagging, because
        // it takes 1/8 s to set texcoords to new sprite desprite changing sequence.
        // But reset():ing it causes the animation to look bad too, because frequent
        // resets when moving makes it seem as if the animation only shows the walking
        // first frame.
        //sequences[sq_name].reset();
    }
}

const std::string & Animation::get_name() const {
    return name;
}

void Animation::set_name(const std::string & n) {
    name = n;
}

TexCoordSequence & Animation::get_current_sequence() {
    return sequences.at(current_sequence_name);
}

const TexCoordSequence & Animation::get_current_sequence() const {
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
        auto sq = entity.animation.get_current_sequence().get_name();
        entity.animation.set_sequence("idle-" + sq.substr(5));
        std::cout << "idle " << sq.substr(5) << std::endl;
    }
    else {
        auto str = "move-" + vec_to_dir[octant];
        std::cout << "set sequence " << str << std::endl;

        auto & seq = entity.animation.get_current_sequence();
        auto frame = seq.get_frame();
        auto dt = entity.animation.get_dt();

        entity.animation.set_sequence(str);
        auto & newseq = entity.animation.get_current_sequence();

        newseq.set_frame(frame);
        entity.animation.set_dt(dt);

        entity.animation.sprite.set_texcoords(newseq.get_texcoords());
    }
}
