#include "animation.hpp"
#include "animationfactory.hpp"
#include "util.hpp"
#include "entity.hpp"
#include "stl.hpp"

TexCoordSequence::TexCoordSequence(sf::IntRect rect, int framecount, int padding) {
    for (int i = 0; i < framecount; i++) {
        frames.emplace_back(rect.left, rect.top, rect.width, rect.height);
        rect.left += rect.width + padding;
    }
    assert(framecount > 0);
}

int TexCoordSequence::advance(int frame) const {
    assert(frames.size() > 0);
    if (is_looping()) {
        return (frame + 1) % frames.size();
    }
    else {
        if (frame < frames.size()) {
            return frame + 1;
        }
        else {
            return frames.size() - 1;
        }
    }
}

void TexCoordSequence::set_name(const std::string & n) {
    name = n;
}

const std::string & TexCoordSequence::get_name() const {
    return name;
}

void TexCoordSequence::set_looping(bool on) {
    loop = on;
}

const sf::IntRect & TexCoordSequence::get_texcoords(int frame) const {
    if (has_ended(frame)) {
        return frames[frames.size() - 1];
    }
    else {
        return frames[frame];
    }
}

const bool TexCoordSequence::is_looping() const {
    return loop;
}

const bool TexCoordSequence::has_ended(int frame) const {
    return is_looping() == false and frame == frames.size();
}

std::string TexCoordSequence::info() const {
    std::stringstream ss; ss << "TexCoordSequence(" << name << ")::[frames="
        << frames.size() << ", loop=" << loop << "]";
    return ss.str();
}

/////////////////////////////////////////////////////////////////////// ANIMATION

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
    clear();
    set_name(other.get_name());
    sprite = other.sprite;
    direction_to_sequence = other.direction_to_sequence;
    action = other.action;
    direction = other.direction;
    current_frame = other.current_frame;
    if (anims == nullptr && other.anims != nullptr) {
        other.anims->add(this);
    }
    if (anims != nullptr) {
        if (other.playing) {
            play();
        }
        else {
            pause();
        }
    }
    return *this;
}

Animation & Animation::operator=(Animation && other) {
    operator=(other);
    other.clear();
    return *this;
}

Animation & Animation::init() {
    reset();
    action = actions::default_action;
    direction = directions::default_direction;
    play();
    assert(sprite.has_impl());
    sprite.set_texcoords(current_sequence->get_texcoords(current_frame));
    return *this;
}

Animation & Animation::reset() {
    current_dt = 0;
    current_frame = 0;
    return *this;
}

Animation & Animation::clear() {
    if (anims != nullptr) {
        anims->remove(this);
    }
    reset();
    sprite.clear();
    anims = nullptr;
    animf = nullptr;
    return *this;
}

Animation & Animation::play() {
    assert(direction_to_sequence != nullptr);
    current_sequence = direction_to_sequence->get(action, direction);
    if (playing == false) {
        playing = true;
    }
    return *this;
}

Animation & Animation::play_action(const actions::Type & a) {
    action = a;
    play();
    return *this;
}

Animation & Animation::set_direction(const directions::Type & dir) {
    direction = dir;
    play();
    return *this;
}

Animation & Animation::pause() {
    playing = false;
    return *this;
}

void Animation::update(float dt) {
    if (playing == false) {
        return;
    }
    current_dt += dt;
    if (current_dt >= frame_duration) {
        current_dt = 0;
        current_frame = current_sequence->advance(current_frame);
        sprite.set_texcoords(current_sequence->get_texcoords(current_frame));
    }
}

const std::string & Animation::get_name() const {
    return name;
}

void Animation::set_name(const std::string & n) {
    name = n;
}

std::string Animation::info() const {
    std::stringstream ss; ss << "Animation(" << get_name() << ")::[dt="
        << current_dt << ", frame=" << current_frame  << ", action=" 
        << action << ", direction=" << direction << "]\n";
    /*
    for (auto & pair : direction_to_sequence) {
        ss << "Action " << pair.first << "\n";
        for (auto & _pair : pair.second) {
            ss << "\t" <<  _pair.second.info() << "\n";
        }
    }
    */
    return ss.str();
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
