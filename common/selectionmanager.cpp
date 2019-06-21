#include "selectionmanager.hpp"

SelectionManager::SelectionManager(SpriteFactory & sf, EntityManager & em)
    : spritef(sf), entitym(em)
{
    sprite = spritef.create("game-ui", "selection-rect");
    sprite.set_layer(config::ui_layer);
    sprite.hide();

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
    sprite.set_screencoords(rect);
    sprite.show();
}

void SelectionManager::set_rect_position(const Position & position) {
    rect.width = position.x - rect.left;
    rect.height = position.y - rect.top;
    sprite.set_screencoords(rect);
}

void SelectionManager::select_current_rect() {
    sprite.hide();
    entities.clear();

    auto selected_entities = entitym.get_in_region(Position::Region{rect});
    for (auto entity : selected_entities) {
        auto & pair = entities.emplace_back(std::make_pair<>(
            entity,
            spritef.create(entity->get_type(), "selection")
        ));
        pair.second.set_layer(config::tile_indicator_layer);
    }
}

void SelectionManager::update() {
    for (auto & pair : entities) {
        Entity & entity = *(pair.first);
        Sprite & selection_sprite = pair.second;
        selection_sprite.set_position(entity.get_position());
    }
}
