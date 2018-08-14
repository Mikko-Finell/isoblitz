#include "animation.hpp"
#include "util.hpp"
#include <sqlite3.h>
#include <cassert>

namespace gfx {
namespace impl {
Frame::Frame(const sf::IntRect & rect) : spritecoords(rect) {
}

sf::IntRect Frame::get_rect() const {
    return spritecoords;
}

sf::Vector2i Frame::get_coords() const {
    return {spritecoords.left, spritecoords.top};
}

sf::Vector2i Frame::get_size() const {
    return {spritecoords.width, spritecoords.height};
}

// Sequence
Sequence::Sequence(int x, int y, int w, int h, int framecount, int padding) {
    for (int i = 0; i < framecount; i++) {
        frames.emplace_back(sf::IntRect{x, y, w, h});
        x += w + padding;
    }
    assert(framecount != 0);
}

void Sequence::init(Sprite & sprite) {
    auto & active_frame = frames[frame];
    sprite.set_spritecoord(active_frame.get_rect());
}

void Sequence::update(time_t dt, Sprite & sprite) {
    assert(frames.size() != 0);

    current_dt += dt;
    if (current_dt >= frame_duration) {
        current_dt = 0;
        frame++;
        frame = frame % frames.size();
        auto & active_frame = frames[frame];
        sprite.set_spritecoord(active_frame.get_rect());
    }
}
void Sequence::reset() {
    current_dt = 0;
    frame = 0;
}
} // impl

// Animation 

Animation::Animation(const std::string & n) : name(n) {
}

void Animation::__init(SpriteManager & spritem) {
    sprite = Sprite{&spritem};
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
    //std::cout << "Setting sequence: " << sq_name << std::endl;
}

// AnimationManager

AnimationManager::AnimationManager(SpriteManager & sm) : spritem(sm) {
    const auto sqlquery = R"(
        SELECT sequences.name, animations.name,
            origin_x+x, origin_y+y, w, h, frames, padding, offset_x, offset_y
        FROM animations INNER JOIN sequences
        ON animations.name = sequences.animation
    )";
    sqlite3 * db;
    sqlite3_stmt * stmt;

    assert(sqlite3_open("../data/testdb.sqlite3", &db) == SQLITE_OK);
    if (sqlite3_prepare(db, sqlquery, -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "SQL Error from AnimationManager, sqlite3_prepare: "
                  << sqlite3_errmsg(db) << std::endl;
        return;
    }

    int result_code = sqlite3_step(stmt);
    while (result_code == SQLITE_ROW) {

        std::string sequence_name{
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0))
        };
        std::string animation_name{
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1))
        };

        const int x = sqlite3_column_int(stmt, 2);
        const int y = sqlite3_column_int(stmt, 3);
        const int w = sqlite3_column_int(stmt, 4);
        const int h = sqlite3_column_int(stmt, 5);
        const int f = sqlite3_column_int(stmt, 6);
        const int p = sqlite3_column_int(stmt, 7);
        const int ox = sqlite3_column_int(stmt, 8);
        const int oy = sqlite3_column_int(stmt, 9);

        animations[animation_name].add_sequence(
            sequence_name, impl::Sequence{x, y, w, h, f, p}
        );

        result_code = sqlite3_step(stmt);
    }
    if(result_code != SQLITE_DONE) {
        std::cerr << "SQL Error: AnimationManager, sqlite3_step: "
            << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

Animation AnimationManager::get(const std::string & name) {
    auto animation = animations.at(name);
    animation.__init(spritem);
    return animation;
}

} // gfx
