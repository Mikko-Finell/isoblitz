#include "animationfactory.hpp"
#include "database.hpp"
#include <iostream>
#include <cassert>

AnimationFactory::AnimationFactory(AnimationSystem & as, SpriteFactory & sf, RenderSystem & rs)
    : anims(as), spritef(sf), default_rs(rs)
{
    auto step_fn = [&](sqlite3_stmt * stmt){
        int column = 0;
        std::string sprite_name{
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, column++))
        };
        std::string sequence_name{
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, column++))
        };
        const int frames = sqlite3_column_int(stmt, column++);
        const int pad = sqlite3_column_int(stmt, column++);
        
        auto init_pair = std::make_pair(sprite_name, sprite_name);
        auto pair = animations.emplace(init_pair);
        auto & animation = animations.at(sprite_name);
        auto sprite_impl = sf.create_impl(sprite_name, sequence_name);

        animation.add_sequence(
            sequence_name, impl::Sequence{sprite_impl.get_texcoords(), frames, pad}
        );
    };

    const auto sqlquery = R"(
        SELECT name, sequence, frames, pad FROM Animation;
    )";
    Database db{"AnimationFactory"};
    db.execute(sqlquery, step_fn);
}

Animation AnimationFactory::create(RenderSystem & rs, const std::string & name) const {
    Animation animation{name, &anims};
    try {
        animation.copy_sequences(animations.at(name));
    }
    catch (std::out_of_range) {
        std::cerr<< "\nERROR: AnimationFactory::get("<<name<<")\n" <<std::endl;
        throw;
    }
    animation.sprite = spritef.create(rs, animation.name(), animation.current_sequence());
    animation.init();
    return animation;
}

Animation AnimationFactory::create(const std::string & name) const {
    return create(default_rs, name);
}
