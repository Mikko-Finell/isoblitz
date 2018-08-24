#include "animationfactory.hpp"
#include "database.hpp"
#include <iostream>
#include <cassert>

AnimationFactory::AnimationFactory(AnimationSystem & as)
    : anims(as)
{
    auto step_fn = [&](sqlite3_stmt * stmt){
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
        const int ox = sqlite3_column_int(stmt, column++);
        const int oy = sqlite3_column_int(stmt, column++);
        const int f = sqlite3_column_int(stmt, column++);
        const int p = sqlite3_column_int(stmt, column++);
        
        auto init_pair = std::make_pair(animation_name, animation_name);
        auto pair = animations.emplace(init_pair);

        auto & animation = animations.at(animation_name);
        animation.sprite.set_offset(ox, oy);
        animation.sprite.set_size(w, h);
        animation.add_sequence(
            sequence_name, impl::Sequence{x, y, w, h, f, p}
        );
    };

    const auto sqlquery = R"(
        SELECT Animation.name, Entity.name,
            Animation.x + Entity.tileset_origin_x,
            Animation.y + Entity.tileset_origin_y,
            Entity.sprite_w, Entity.sprite_h,
            Entity.sprite_offset_x, Entity.sprite_offset_y,
            frames, pad
        FROM Entity INNER JOIN Animation
        ON Entity.name = Animation.entity
    )";
    Database db{"AnimationFactory"};
    db.execute(sqlquery, step_fn);
}

Animation AnimationFactory::get(const type_id_t & type) const {
    Animation animation{type};
    try {
        animation = animations.at(type);
    }
    catch (std::out_of_range) {
        std::cerr<< "\nERROR: AnimationFactory::get("<<type<<")\n" <<std::endl;
        throw;
    }
    animation.init();
    anims.add(animation);
    return animation;
}
