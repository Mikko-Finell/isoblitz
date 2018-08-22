#include "entitymanager.hpp"

EntityManager::EntityManager(EntityFactory & ef) : entityf(ef) {
}

Entity * EntityManager::create(const type_id_t & type) {
    if (is_updating) {
        throw std::logic_error{"create while updating"};
    }
    else {
        entities.push_back(entityf.get(type));
    }
    return &entities.back();
}

void EntityManager::remove(const uid_t & id) {
    auto cmp = [id](const Entity & e){ return e.get_uid() == id; };
    auto itr = std::find_if(entities.begin(), entities.end(), cmp);

    if (itr != entities.end()) {
        if (is_updating) {
            delete_queue.push_back(itr);
        }
        else {
            entities.erase(itr);
        }
    }
}

void EntityManager::update(time_t dt) {
    is_updating = true;
    for (auto & entity : entities) {
        entity.update(dt);
    }
    for (auto & itr : delete_queue) {
        entities.erase(itr);
    }
    delete_queue.clear();
    is_updating = false;
}

void EntityManager::serialize(std::ostream & out) const {
    util::write(entities.size(), out);
    for (auto & entity : entities) {
        entity.serialize(out);
    }
}

void EntityManager::deserialize(std::istream & in, AnimationFactory & animf) {
    decltype(entities.size()) size;
    util::read(size, in);
    for (auto i = 0; i < size; i++) {
        entities.emplace_back(in, animf);
        // TODO probably have to .init here
    }
}
