#include "map.hpp"
#include "entity.hpp"
#include "selection.hpp"
#include "common/input.hpp"
#include "common/camera.hpp"
#include "common/animation.hpp"
#include "common/spritefactory.hpp"
#include <iostream>

void init(sf::RenderWindow & window, Camera & camera, input::Manager & inputm,
          RenderSystem & render, SpriteFactory & spritef, 
          AnimationManager & anim, Map & map);

int main() {
    static sf::RenderWindow window;
    static Camera camera{window};
    static input::Manager inputm{window};
    static RenderSystem render;
    static SpriteFactory spritef;
    static AnimationManager anim{render};
    static Map map{render};
    init(window, camera, inputm, render, spritef, anim, map);

    //camera.zoom(2.0);
    map.load("testmap.bulletmap");

    Entity entity;
    entity.set_cell(cell_t{1, 1});

    entity.animation = anim.get("test");
    entity.animation.set_sequence("idle-down");
    // TODO figure out automatic way of setting this
    entity.animation.sprite.set_layer(2);

    while (window.isOpen()) {
        inputm.poll_sfevents();

        entity.update(16);

        window.clear(sf::Color::White);
        render.draw(window);
        window.display();
    }
}

void init(sf::RenderWindow & window, Camera & camera, input::Manager & inputm,
          RenderSystem & render, SpriteFactory & spritef, 
          AnimationManager & anim, Map & map)
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

    render.load_texture("../sprites/sprites.png");

    map.signal.map_loaded.add_callback([&](int w, int h){
        const sf::Vector2f v(w * 0.5f, h * 0.5f);
        auto u = util::to_pixel(v);
        camera.focus_at(util::to_pixel(v));
    });

    auto scroll = [&](const input::Event & event){
        if (inputm.is_button_pressed(sf::Mouse::Middle)) {
            // note: mousedt is the actual change, and so moving the camera by
            // that amount makes it look like the world is scrolling in the 
            // opposite direction, multiply by -1 to make it look like we are
            // moving the world instead of the camera 
            camera.scroll(sf::Vector2f(-1 * event.get_mousedt()));
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

    static SelectionManager selectm{render, spritef};
    static input::Context selectionctx;

    input::Event sel_event{sf::Event::MouseButtonPressed};
    sel_event.set_button(sf::Mouse::Left);
    selectionctx.bind(sel_event, [&](const input::Event & event){
        selectm.start(event.get_mousepos());
        return true;
    });

    sel_event = input::Event{sf::Event::MouseMoved};
    selectionctx.bind(sel_event, [&](const input::Event & event){
        if (inputm.is_button_pressed(sf::Mouse::Left)) {
            selectm.update(event.get_mousepos());
            return true;
        }
        return false;
    });

    sel_event = input::Event{sf::Event::MouseButtonReleased};
    sel_event.set_button(sf::Mouse::Left);
    selectionctx.bind(sel_event, [&](const input::Event & event){
        selectm.select_current_rect();
        return true;
    });

    static Sprite hlsprite{render};
    hlsprite = spritef.get("game", "tile-indicator");
    hlsprite.set_layer(2);

    // highlight tile from mouse movement
    auto hl = [&](const input::Event & event) -> bool {
        auto mousepos = event.get_mousepos();
        sf::Vector2f logicpos = util::to_grid<>(mousepos);
        if (auto tile = map.get_tile(logicpos); tile != nullptr) {
            // TODO find a good way to do this
            auto x = tile->sprite.data.screencoords.left;
            auto y = tile->sprite.data.screencoords.top;
            hlsprite.set_position(x, y);
            hlsprite.show();
        }
        else {
            hlsprite.hide();
        }
        return false;
    };

    static input::Context tilectx;
    tilectx.bind(input::Event{sf::Event::MouseMoved}, hl);

    inputm.push_context(tilectx);
    inputm.push_context(selectionctx);
}
