#include "animationfactory.hpp"
#include "database.hpp"
#include <iostream>
#include <cassert>

AnimationFactory::AnimationFactory(AnimationManager & am, AnimationSystem & as, SpriteFactory & sf)
    : animm(am), anims(as), spritef(sf)
{
    auto step_fn = [&](sqlite3_stmt * stmt){
        int column = 0;
        std::string animation_name{
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, column++))
        };
        std::string sequence_name{
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, column++))
        };
        const int frames = sqlite3_column_int(stmt, column++);
        const int pad = sqlite3_column_int(stmt, column++);
        
        auto init_pair = std::make_pair(animation_name, animation_name);
        auto pair = animations.emplace(init_pair);
        auto & animation = animations.at(animation_name);
        auto sprite = sf.create(animation_name, sequence_name);

        animation.add_sequence(
            sequence_name, impl::Sequence{sprite.get_spritecoords(), frames, pad}
        );
    };

    const auto sqlquery = R"(
        SELECT name, sequence, frames, pad FROM Animation;
    )";
    Database db{"AnimationFactory"};
    db.execute(sqlquery, step_fn);
}

Animation * AnimationFactory::create(RenderSystem & rs, const type_id_t & type) const {
    Animation * animation = animm.alloc();
    try {
        animation->copy_sequences(animations.at(type));
    }
    catch (std::out_of_range) {
        animm.destroy(animation);
        std::cerr<< "\nERROR: AnimationFactory::get("<<type<<")\n" <<std::endl;
        throw;
    }
    animation->sprite = spritef.create(rs, animation->name(), animation->current_sequence());
    animation->init();
    anims.add(animation);
    return animation;
}

