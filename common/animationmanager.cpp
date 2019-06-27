#include "animationmanager.hpp"

namespace actions {
static const std::string move {"move"};
static const std::string idle {"idle"}; 
static const std::string fight{"fight"};
static const std::string die  {"die"};
}
namespace directions {
static const std::string down       {"down"};
static const std::string up         {"up"};
static const std::string left       {"left"};
static const std::string right      {"right"};
static const std::string down_right {"down-right"};
static const std::string down_left  {"down-left"};
static const std::string up_right   {"up-right"};
static const std::string up_left    {"up-left"};
static const std::string map[8] =   {
    down, down_left, left, up_left, up, up_right, right, down_right
};
std::string from_vector(const sf::Vector2f & vector) {
    // copypasted unchecked code from stackexchange
    enum compassDir {
        left = 0, up_left = 1, up = 2, up_right = 3, 
        right = 4, down_right = 5, down = 6, down_left = 7
    };
    // actual conversion code:
    float angle = atan2( vector.y, vector.x );
    // add 45 degrees because of isometric rotation
    angle -= M_PI/4;
    int octant = int( 8 * angle / (2*M_PI) + 8.5 ) % 8;
    compassDir dir = (compassDir) octant;  // typecast to enum: 0 -> E etc.
    return directions::map[octant];
}
}

void AnimationManager::add_entity(Entity & entity) {
    direction[&entity] = directions::down;
    action[&entity] = actions::idle;
}

void AnimationManager::remove_entity(Entity & entity) {
    direction.erase(&entity);
    action.erase(&entity);
}

void AnimationManager::update(Entity & entity) {
}

void AnimationManager::on_entity_move(Entity & entity, const sf::Vector2f & vector) {
    if (vector.x == 0 and vector.y == 0) {
        return on_entity_idle(entity);
    }
    else {
        entity.animation.play_action(actions::move)
                        .set_direction(directions::from_vector(vector));
        /*
        auto direction = directions::from_vector(vector);
        animation.set_direction(directios::from_vector(vector);
        if (animation.get_action() == actions::move) {
            if (animation.get_direction() == direction) {
                return; // already moving in same direction
            }
            else { // already moving, but change direction
                animation.set_direction(direction);
            }
        }
        else {
            animation.play_action(actions::move);
            animation.set_direction(direction);
        }
        */
    }
}

void AnimationManager::on_entity_idle(Entity & entity) {
    entity.animation.reset().play_action(actions::idle);
}

void AnimationManager::on_entity_fight(Entity & entity) {
    entity.animation.reset().play_action(actions::fight);
}

void AnimationManager::on_entity_die(Entity & entity) {
    entity.animation.reset().play_action(actions::die);
}
