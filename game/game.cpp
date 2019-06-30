#include "minimap.hpp"
#include "common/engine.hpp"
#include "CASE/timer.hpp"
#include <iostream>

int main() {
    Engine engine;

    input::Context testctx;
    engine.inputm.push_context(engine.selectm.ctx);
    engine.inputm.push_context(testctx);

    input::Event TEST{sf::Event::MouseButtonPressed};
    TEST.set_button(sf::Mouse::Right);
    testctx.bind(TEST, [&](const input::Event & event) {
        engine.selectm.map([&](Entity & entity){
            engine.pathm.find_path(entity, event.get_mousepos_logic().to_grid());
        });
        return true;
    });
    TEST = input::Event{sf::Event::KeyPressed};
    TEST.set_key(sf::Keyboard::Delete);
    testctx.bind(TEST, [&](const input::Event & event) {
        engine.selectm.map([&](Entity & entity){
            engine.entitym.queue_destroy(entity);
        });
        return true;
    });

    engine.moves.signals.entity_move.add_observer(engine.animm, &AnimationManager::on_entity_move);
    engine.load();
    Minimap minimap{engine};
    engine.run();
}
