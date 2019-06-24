#include "entitysystem.hpp"

void EntitySystem::_update(float dt) {
    for (Entity * entity : _entities) {
        _update_single(*entity);
    }
}

void EntitySystem::_update_single(Entity & entity) {
}

void EntitySystem::_add_entity(Entity &) {
}

void EntitySystem::_remove_entity(Entity &) {
}

void EntitySystem::_clear() {
    for (Entity * entity : _entities) {
        remove_entity(entity);
    }
}

EntitySystem::~EntitySystem() {
}

void EntitySystem::update(float dt) {
    for (Entity * entity : _remove_queue) {
        remove_entity(entity);
    }
    _remove_queue.clear();
    _update(dt);
}

void EntitySystem::add_entity(Entity & entity) {
    if (_entities.insert(&entity).second == true) {
        entity.signals.im_dead.add_observer(this, [&](Entity & entity){
            remove_entity(entity);
        });
        _add_entity(entity);
    }
}

void EntitySystem::remove_entity(Entity & entity) {
    _entities.erase(&entity);
    unsubscribe(&entity.signals.im_dead);
    _remove_entity(entity);
}

void EntitySystem::queue_remove(Entity & entity) {
    if (_entities.count(&entity) == 1) {
        _remove_queue.insert(&entity);
    }
}

void EntitySystem::clear() {
    _clear();
}
