#include "animation.hpp"
#include "util.hpp"
#include <sqlite3.h>
#include <iostream>
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

// Animation ////////////////////////////////////////////////////////////////////

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

// AnimationManager /////////////////////////////////////////////////////////////

AnimationManager::AnimationManager(RenderSystem & rs) : render(rs) {
    const auto sqlquery = R"(
        SELECT sprite.name, entity.name,
            origin_x+x, origin_y+y, w, h, frames, padding, offset_x, offset_y
        FROM entity INNER JOIN sprite
        ON entity.name = sprite.entity
        WHERE sprite.frames NOT NULL
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
        int column = 0;

        std::string sequence_name{
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, column++))
        };
        std::string animation_name{
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, column++))
        };

        const int x = sqlite3_column_int(stmt, column++);
        const int y = sqlite3_column_int(stmt, column++);
        const int w = sqlite3_column_int(stmt, column++);
        const int h = sqlite3_column_int(stmt, column++);
        const int f = sqlite3_column_int(stmt, column++);
        const int p = sqlite3_column_int(stmt, column++);
        const int ox = sqlite3_column_int(stmt, column++);
        const int oy = sqlite3_column_int(stmt, column++);

        auto & animation = animations[animation_name];
        animation.sprite.set_offset(ox, oy);
        animation.sprite.set_size(w, h);
        animation.add_sequence(
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
    animation.init(render);
    return animation;
}
