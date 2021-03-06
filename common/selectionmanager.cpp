#include "selectionmanager.hpp"
#include "stl.hpp"

void SelectionManager::update(float db) {
    for (auto entity : entities) {
        if (entity_sprite_map.count(entity) == 1) {
            Sprite & sprite = entity_sprite_map[entity];
            sprite.set_position(entity->get_position());
        }
    }
}

void SelectionManager::remove_entity(Entity & entity) {
    entities.erase(&entity);
    entity_sprite_map.erase(&entity);
}

void SelectionManager::clear() {
    entities.clear();
    entity_sprite_map.clear();
    selection_sprite.hide();
}

SelectionManager::SelectionManager(SpriteFactory & sf, EntityManager & em)
    : spritef(sf), entitym(em)
{
    selection_sprite = spritef.create("selection-rect");
    selection_sprite.set_layer(config::ui_layer);
    selection_sprite.hide();

    input::Event sel_event{sf::Event::MouseButtonPressed};
    sel_event.set_button(sf::Mouse::Left);
    ctx.bind(sel_event, [&](const input::Event & event){
        start(event.get_mousepos_pixel());
        return true;
    });

    sel_event = input::Event{sf::Event::MouseMoved};
    ctx.bind(sel_event, [&](const input::Event & event){
        if (ctx.is_button_pressed(sf::Mouse::Left)) {
            set_rect_position(event.get_mousepos_pixel());
            return true;
        }
        return false;
    });

    sel_event = input::Event{sf::Event::MouseButtonReleased};
    sel_event.set_button(sf::Mouse::Left);
    ctx.bind(sel_event, [&](const input::Event & event){
        select_current_rect();
        return true;
    });
}

void SelectionManager::start(const Position & position) {
    rect = sf::FloatRect(position.x, position.y, 1.0f, 1.0f);
    selection_sprite.set_screencoords(rect);
    selection_sprite.show();
}

void SelectionManager::set_rect_position(const Position & position) {
    rect.width = position.x - rect.left;
    rect.height = position.y - rect.top;
    selection_sprite.set_screencoords(rect);
}

void SelectionManager::select_current_rect() {
    system_clear(); 
    auto selected_entities = entitym.get_in_region(Position::Region{rect});
    for (auto entityptr : selected_entities) {
        system_add_entity(entityptr);

        try {
            entity_sprite_map[entityptr] = spritef.create(entityptr->get_type() + "-selection");
            Sprite & sprite = entity_sprite_map[entityptr];
            sprite.set_layer(config::tile_indicator_layer);
        }
        catch (std::out_of_range) {
            std::cerr << "WARNING " << entityptr->get_type() << " has no selection indicator.\n";
        }
        entities.insert(entityptr);
    }
}

void SelectionManager::map(const std::function<void(Entity &)> & fn) {
    for (auto entity : entities) {
        fn(*entity);
    }
}
