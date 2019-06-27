#include "animationfactory.hpp"
#include "database.hpp"
#include <iostream>
#include <cassert>

ActionToDirectionMap::DirectionSequenceMap & 
ActionToDirectionMap::operator[](const actions::Type & action) {
    return action_to_dirmap[action];
}

TexCoordSequence * 
ActionToDirectionMap::get(const actions::Type & action, const directions::Type & dir) 
{
    auto & dir_to_sequencemap = action_to_dirmap.at(action);
    return &dir_to_sequencemap.at(dir);
}

AnimationFactory::AnimationFactory(AnimationSystem & as, SpriteFactory & sf, RenderSystem & rs)
    : anims(as), spritef(sf), default_rs(rs)
{
    auto step_fn = [&](sqlite3_stmt * stmt){
        int column = 0;
        std::string entity_type{
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, column++))
        };
        std::string action_type{
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, column++))
        };
        std::string sequence_name{
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, column++))
        };
        const int frames = sqlite3_column_int(stmt, column++);
        const int pad = sqlite3_column_int(stmt, column++);
        const int loop = sqlite3_column_int(stmt, column++);
        const int x = sqlite3_column_int(stmt, column++);
        const int y = sqlite3_column_int(stmt, column++);
        const int w = sqlite3_column_int(stmt, column++);
        const int h = sqlite3_column_int(stmt, column++);
        
        TexCoordSequence sequence{sf::IntRect{x, y, w, h}, frames, pad};
        sequence.set_looping(loop);
        sequence.set_name(sequence_name);

        auto & action_to_dirmap = entity_to_actionmap[entity_type];
        auto & dir_to_sequencemap = action_to_dirmap[action_type];
        dir_to_sequencemap.emplace(sequence_name, sequence);
    };

    const auto sqlquery = R"(
        SELECT 
            Animation.entity_type, Animation.action_type, Animation.sequence_name, 
            Animation.frames, Animation.pad, Animation.loop, 
            Animation.region_x, Animation.region_y, Sprite.w, Sprite.h
        FROM Animation INNER JOIN Sprite
        ON Animation.entity_type = Sprite.name;
    )";
    Database db{"AnimationFactory"};
    db.execute(sqlquery, step_fn);
}

Animation AnimationFactory::create(RenderSystem & rs, const std::string & name) const {
    Animation animation{name, &anims};
    animation.direction_to_sequence = &entity_to_actionmap[name];
    try {
        //animation.copy_sequences(animations.at(name));
    }
    catch (std::out_of_range) {
        std::cerr<< "\nERROR: AnimationFactory::get("<<name<<")\n" <<std::endl;
        throw;
    }
    animation.sprite = spritef.create(rs, name);
    animation.init();
    return animation;
}

Animation AnimationFactory::create(const std::string & name) const {
    return create(default_rs, name);
}
