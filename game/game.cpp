#include "map.hpp"
#include "common/input.hpp"
#include "common/animation.hpp"
#include <iostream>

int main() {
    sf::RenderWindow window;
    window.create(sf::VideoMode{800, 800}, "Bullet Broodwar");
    window.setFramerateLimit(60);
    //window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);

    input::Manager inputm{window};
    inputm.create_action("quit", [&](){ window.close(); });

    input::Context gctx;
    inputm.push_context(gctx);
    gctx.bind(input::Event{sf::Event::Closed}, "quit");

    input::Event keyp{sf::Event::KeyPressed};
    keyp.set_key(sf::Keyboard::Q);
    gctx.bind(keyp, "quit");

    gfx::SpriteManager spritem;
    spritem.texture.loadFromFile("../sprites/sprites128x64.png");

    Map map{"tmp.bulletmap", spritem};

    auto scroll = [&](const input::Event & event){
        if (inputm.is_button_pressed(sf::Mouse::Middle)) {
            auto view = window.getView();
            view.move(sf::Vector2f(event.get_mousedt()));
            window.setView(view);
            return true;
        }
        return false;
    };
    gctx.bind(input::Event{sf::Event::MouseMoved}, scroll);

    gfx::Sprite hltile{&spritem};
    hltile.set_spritecoord({256, 128});
    hltile.set_origin(sf::Vector2i(0, -(SPRIH/4)));
    hltile.set_size({SPRIW, SPRIH});
    hltile.set_layer(2);

    // highlight tile from mouse movement
    auto hl = [&](const input::Event & event) -> bool {
        sf::Vector2i mousepos = event.get_mousepos();
        sf::Vector2f coordpos = window.mapPixelToCoords(sf::Vector2i(mousepos));
        coordpos.x -= TILEW / 2;
        sf::Vector2f logicpos = to_grid(coordpos);
        if (auto tile = map.get_tile(logicpos); tile != nullptr) {
            hltile.set_visible(true);
            hltile.set_position(logic_to_pixel(tile->coordinate()));
        }
        else {
            hltile.set_visible(false);
        }
        return false;
    };

    input::Context tilectx;
    tilectx.bind(input::Event{sf::Event::MouseMoved}, hl);
    inputm.push_context(tilectx);



    while (window.isOpen()) {
        inputm.poll_sfevents();
        window.clear(sf::Color::Black);
        spritem.draw(window);
        window.display();
    }
}
