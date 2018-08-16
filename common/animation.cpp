#include "animation.hpp"
#include "util.hpp"
#include <cassert>

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
    assert(frames.size() != 0);

    current_dt += dt;
    if (current_dt >= frame_duration) {
        current_dt = 0;
        frame++;
        frame = frame % frames.size();
        sprite.set_spritecoords(frames[frame]);
    }
}
void Sequence::reset() {
    current_dt = 0;
    frame = 0;
}
} // impl

Animation::Animation(const std::string & n) : name(n) {
}

void Animation::init(RenderSystem & render) {
    sprite.init(render);
    for (auto & pair : sequences) {
        pair.second.reset();
    }
    set_sequence(sequences.begin()->first);
    active_sequence->init(sprite);
}

void Animation::update(time_t dt) {
    assert(active_sequence);
    active_sequence->update(dt, sprite);
}

void 
Animation::add_sequence(const std::string & sq_name, const impl::Sequence & sq) {
    sequences[sq_name] = sq;
}

void Animation::set_sequence(const std::string & sq_name) {
    if (active_sequence) {
        active_sequence->reset();
    }
    active_sequence = &sequences.at(sq_name);
    active_sequence->init(sprite);
}
