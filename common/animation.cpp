#include "animation.hpp"
#include <sqlite3.h>
#include <cassert>

namespace gfx {
namespace impl {
sf::IntRect Frame::get_rect() const {
    return spritecoords;
}

sf::Vector2i Frame::get_coords() const {
    return {spritecoords.left, spritecoords.top};
}

// Sequence

void Sequence::update(time_t dt) {
}

void Sequence::set_loop(bool b) {
}

void Sequence::set_next(const std::string & name) {
}

std::string Sequence::next_sequence() const {
    return next;
}

bool Sequence::finished() const {
    return frame == frames.size();
}

const Frame & Sequence::current_frame() const {
    return frames[frame];
}
} // impl

// Animation 
void Animation::update(time_t dt) {
    active_sequence->update(dt);

    if (active_sequence->finished()) {
        current_sequence = active_sequence->next_sequence();
        active_sequence = &sequences[current_sequence];
    }
    else {
        set_spritecoord(active_sequence->current_frame().get_coords());
    }
}

void Animation::add_sequence(const std::string & name, const impl::Sequence & s)
{
    sequences[name] = s;
}

void Animation::set_sequence(const std::string & name) {
    auto itr = sequences.find(name);
    assert(itr != sequences.end());
    current_sequence = name;
    active_sequence = &sequences[name];
}

// AnimationManager

AnimationManager::AnimationManager(SpriteManager & sm) : spritem(sm) {
    const auto sqlquery = "SELECT * FROM animations";
    sqlite3 * db;
    sqlite3_stmt * stmt;

    assert(sqlite3_open("../data/testdb.sqlite3", &db) == SQLITE_OK);
    if (sqlite3_prepare(db, sqlquery, -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "ERROR: while compiling sql: "
                  << sqlite3_errmsg(db) << std::endl;
        return;
    }

    int ret_code = sqlite3_step(stmt);
    while (ret_code == SQLITE_ROW) {
        std::string name;
        name = *sqlite3_column_text(stmt, 0);
        const int frames = sqlite3_column_int(stmt, 1);
        const int x = sqlite3_column_int(stmt, 2);
        const int y = sqlite3_column_int(stmt, 3);
        const int w = sqlite3_column_int(stmt, 4);
        const int h = sqlite3_column_int(stmt, 5);

        ret_code = sqlite3_step(stmt);
    }
    if(ret_code != SQLITE_DONE) {
        //this error handling could be done better, but it works
        std::cerr << "ERROR: while performing sql: " << sqlite3_errmsg(db) << std::endl;
        printf("ret_code = %d\n", ret_code);
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

} // gfx
