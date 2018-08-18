#include "animationfactory.hpp"
#include "database.hpp"
#include <functional>
#include <iostream>
#include <cassert>

namespace {
std::function<void(sqlite3_stmt * stmt)> step_fn;
}

AnimationFactory::AnimationFactory(RenderSystem & rs) : render(rs) {
    step_fn = [&](sqlite3_stmt * stmt){
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
    };

#ifndef LAZY_FACTORY 

    const auto sqlquery = R"(
        SELECT Animation.name, Entity.name,
            Animation.x + Entity.tileset_origin_x,
            Animation.y + Entity.tileset_origin_y,
            Entity.sprite_w, Entity.sprite_h,
            frames, pad
        FROM Entity INNER JOIN Animation
        ON Entity.name = Animation.entity
    )";
    Database db{"AnimationFactory"};
    db.execute(sqlquery, step_fn);

#endif
}

Animation AnimationFactory::get(const std::string & name) {
    Animation animation;

#ifdef LAZY_FACTORY

    if (auto itr = animations.find(name); itr == animations.end()) {
        const auto sqlquery = R"(

            WHERE Sprite.entity = ?
        )";
        Database db{"AnimationFactory::dynamic_fetch"};
        auto stmt = db.prepare(sqlquery);
        sqlite3_bind_text(stmt, 1, name.c_str(), -1, NULL);
        db.execute(step_fn);
    }

#endif

    try {
        animation = animations.at(name);
    }
    catch (std::out_of_range) {
        std::cerr<< "\nERROR: AnimationFactory::get("<<name<<")\n" <<std::endl;
        throw;
    }

    animation.init(render);
    return animation;
}
