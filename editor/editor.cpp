#include "tileedit.hpp"
#include "entityedit.hpp"
#include "common/state.hpp"
#include "common/serialize.hpp"
#include <CASE/timer.hpp>
#include <iostream>
#include <memory>

int main(int argc, char * argv[]) {
    auto & engine = StateManager::create("Editor");

    std::unique_ptr<TileEdit> tileptr{nullptr};
    std::unique_ptr<EntityEdit> entityptr{nullptr};

    input::Event event{sf::Event::KeyPressed};
    event.set_key(sf::Keyboard::T);
    auto globctx = engine.inputm.get_global_context();
    globctx->bind(event, [&](){
        if (tileptr == nullptr) {
            tileptr.reset(new TileEdit{engine});
            entityptr.reset(nullptr);
        }
    });

    event.set_key(sf::Keyboard::E);
    globctx->bind(event, [&](){
        if (entityptr == nullptr) {
            tileptr.reset(nullptr);
            entityptr.reset(new EntityEdit{engine});
        }
    });

    event.set_key(sf::Keyboard::N);
    event.set_mod(input::Mod::CTRL, true);
    globctx->bind(event, [&](){
        engine.tilem.clear();
    });

    tileptr.reset(new TileEdit{engine});

    try {
        IOReader in{"../maps/tmp.isoblitz"};
        engine.camera.deserialize(in);
        engine.tilef.deserialize(in);
    }
    catch (std::invalid_argument) {
        std::cerr << "Unable to load tmp.isoblitz." << std::endl;
    }
    engine.run();
    try {
        IOWriter out{"../maps/tmp.isoblitz"};
        engine.camera.serialize(out);
        engine.tilem.serialize(out);
    }
    catch (...) {
        std::cerr << "Unable to save tmp.isoblitz." << std::endl;
    }
}
