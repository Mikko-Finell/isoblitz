#include "entitymanager.hpp"
#include "entityfactory.hpp"
#include <iostream>

EntityManager::EntityManager(AnimationManager & am) : animm(am) {
}

Entity * EntityManager::alloc() {
    entities.emplace_back();
    std::cout << "Created " << &entities.back() << std::endl;
    return &entities.back();
}

void EntityManager::destroy(Entity * entity) {
    assert(get(entity->uid()) == entity);
    auto cmp = [entity](const Entity & e){ return entity->uid() == e.uid(); };
    auto itr = std::find_if(entities.begin(), entities.end(), cmp);
    if (itr != entities.end()) {
        animm.destroy(entity->animation);
        entities.erase(itr);
        std::cout << "Erased " << entity << std::endl;
    }
}

// TODO easy
// should it error if uid not in manager?
Entity * EntityManager::get(const uid_t & uid) {
    auto cmp = [uid](const Entity & entity){ return entity.uid() == uid; };
    auto itr = std::find_if(entities.begin(), entities.end(), cmp);
    if (itr != entities.end()) {
        return &(*itr);
    }
    return nullptr;
}

std::vector<Entity *> EntityManager::get_all() {
    std::vector<Entity *> vec;
    for (auto entity : entities) {
        vec.push_back(&entity);
    }
    return vec;
}

void EntityManager::clear() {
    for (auto & e : entities) {
        destroy(&e);
    }
}

void EntityManager::serialize(std::ostream & out) const {
    throw std::logic_error{"Entity::serialize not implemented."};
    /*
    util::write(entities.size(), out);
    for (auto entity : entities) {
        util::serialize_std_string(entity.get_type(), out);
        entity.cell.serialize(out);

        util::serialize_std_string(entity->animation->current_sequence(), out);
    }
    */
}

void EntityManager::deserialize(EntityFactory * ef, std::istream & in) {
    throw std::logic_error{"Entity::deserialize not implemented."};
    /*
    clear();
    decltype(entities.size()) entity_count;
    util::read(entity_count, in);
    for (auto i = 0; i < entity_count; i++) {
        type_id_t type = util::deserialize_std_string(in);

        auto entity = ef.get(type);
        add_entity(entity);

        entity->cell.deserialize(in);

        auto sequence = util::deserialize_std_string(in);
        entity->animation->set_sequence(sequence, "EntityManager::deserialize");

        entity->set_cell(entity->cell); // to update sprite pos
    }
    */
}
