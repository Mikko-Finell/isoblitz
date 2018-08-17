#include "animationfactory.hpp"
#include "database.hpp"
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
    Database db{"AnimationFactory"};
    db.execute(sqlquery, [&](sqlite3_stmt * stmt){ 
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
    });
}

Animation AnimationFactory::get(const std::string & name) {
    Animation animation;
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
