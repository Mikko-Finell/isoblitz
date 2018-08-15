#include "map.hpp"
#include "entity.hpp"
#include "common/input.hpp"
#include "common/camera.hpp"
#include "common/animation.hpp"
#include <iostream>

void init(sf::RenderWindow & window, Camera & camera, input::Manager & inputm,
          gfx::SpriteManager & spritem, gfx::AnimationManager & anim, Map & map);

int main() {
    sf::RenderWindow window;
    Camera camera{window};
    input::Manager inputm{window};
    gfx::SpriteManager spritem;
    gfx::AnimationManager anim{spritem};
    Map map{spritem};
    init(window, camera, inputm, spritem, anim, map);

    camera.zoom(2.0);
    map.load("testmap.bulletmap");

    Entity entity;
    entity.animation = anim.get("test");
    entity.animation.set_sequence("idle-down");
    //entity.animation.sprite.set_layer(ENTITY_LAYER); // TODO automize this
    entity.set_cell(cell_t{1, 1});

    while (window.isOpen()) {
        inputm.poll_sfevents();

        entity.update(16);

        window.clear(sf::Color::White);
        spritem.draw(window);
        window.display();
    }
}

void init(sf::RenderWindow & window, Camera & camera, input::Manager & inputm,
          gfx::SpriteManager & spritem, gfx::AnimationManager & anim, Map & map)
{
    window.create(sf::VideoMode{WINW, WINH}, "Bullet Broodwar");
    window.setFramerateLimit(60);
    //window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);
    util::center_window(window);

    inputm.create_action("quit", [&](){ window.close(); });

    static input::Context gctx;
    inputm.push_context(gctx);
    gctx.bind(input::Event{sf::Event::Closed}, "quit");

    input::Event keyp{sf::Event::KeyPressed};
    keyp.set_key(sf::Keyboard::Q);
    gctx.bind(keyp, "quit");

    spritem.texture.loadFromFile("../sprites/sprites.png");

    map.signal.map_loaded.add_callback([&](int w, int h){
        const sf::Vector2f v(w * 0.5f, h * 0.5f);
        auto u = util::to_pixel(v);
        camera.focus_at(util::to_pixel(v));
    });

    auto scroll = [&](const input::Event & event){
        if (inputm.is_button_pressed(sf::Mouse::Middle)) {
            camera.scroll(sf::Vector2f(event.get_mousedt()));
            return true;
        }
        return false;
    };
    gctx.bind(input::Event{sf::Event::MouseMoved}, scroll);

    input::Event zoom{sf::Event::MouseWheelScrolled};
    gctx.bind(zoom, [&](const input::Event & event){
        if (inputm.is_button_pressed(sf::Mouse::Middle)) {
            return false;
        }
        constexpr float zoomfactor = 2.0f;
        //if (inputm.is_key_pressed(sf::Keyboard::LControl)) {
            if (event.get_scroll() < 0) {
                camera.zoom(1/zoomfactor);
            }
            else {
                camera.zoom(zoomfactor);
            }
            return true;
        //}
        //return false;
    });
#if 0
    static gfx::Sprite hlsprite{&spritem};
    hlsprite.set_spritecoord({128, 128});
    hlsprite.set_layer(2);
    hlsprite.set_visible(false);

    // highlight tile from mouse movement
    auto hl = [&](const input::Event & event) -> bool {
        sf::Vector2i mousepos = event.get_mousepos();
        sf::Vector2f coordpos = window.mapPixelToCoords(sf::Vector2i(mousepos));
        sf::Vector2f logicpos = util::to_grid(coordpos);
        if (auto tile = map.get_tile(logicpos); tile != nullptr) {
            hlsprite.set_visible(true);
            hlsprite.set_position(tile->get_sprite().position());
        }
        else {
            hlsprite.set_visible(false);
        }
        return false;
    };
    static input::Context tilectx;
    tilectx.bind(input::Event{sf::Event::MouseMoved}, hl);
    inputm.push_context(tilectx);
#endif
}
