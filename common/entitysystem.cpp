#include "entitysystem.hpp"

void EntitySystem::update(float dt) {
}

void EntitySystem::add_entity(Entity &) {
}

void EntitySystem::remove_entity(Entity &) {
}

void EntitySystem::clear() {
    for (Entity * entity : _entities) {
        system_remove_entity(entity);
    }
}

EntitySystem::~EntitySystem() {
}

void EntitySystem::system_update(float dt) {
    for (Entity * entity : _remove_queue) {
        system_remove_entity(entity);
    }
    _remove_queue.clear();
    update(dt);
}

void EntitySystem::system_add_entity(Entity & entity) {
    if (_entities.insert(&entity).second == true) {
        entity.signals.im_dead.add_observer(this, [&](Entity & entity){
            system_remove_entity(entity);
        });
        add_entity(entity);
    }
}

void EntitySystem::system_remove_entity(Entity & entity) {
    _entities.erase(&entity);
    unsubscribe(&entity.signals.im_dead);
    remove_entity(entity);
}

void EntitySystem::queue_remove(Entity & entity) {
    if (_entities.count(&entity) == 1) {
        _remove_queue.insert(&entity);
    }
}

void EntitySystem::system_clear() {
    clear();
}
