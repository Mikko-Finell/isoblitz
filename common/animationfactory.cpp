#include "animationfactory.hpp"
#include <sqlite3.h>
#include <iostream>
#include <cassert>

AnimationFactory::AnimationFactory(RenderSystem & rs) : render(rs) {
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
        std::cerr << "SQL Error from AnimationFactory, sqlite3_prepare: "
                  << sqlite3_errmsg(db) << std::endl;
        std::terminate();
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
        std::cerr << "SQL Error: AnimationFactory, sqlite3_step: "
                  << sqlite3_errmsg(db) << std::endl;
        std::terminate();
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

Animation AnimationFactory::get(const std::string & name) {
    auto animation = animations.at(name);
    animation.init(render);
    return animation;
}
