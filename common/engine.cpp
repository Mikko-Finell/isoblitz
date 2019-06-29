#include "engine.hpp"
#include "util.hpp"
#include "state.hpp"
#include <CASE/timer.hpp>
#include <iostream>

void Engine::init() {
    camera.center_window(1920, 1080, config::winw, config::winh);

    auto & globctx = *inputm.get_global_context();
    globctx.bind("terminate", [&](){
        StateManager::terminate();
        sfml.window.close();
    });
    input::Event event{sf::Event::Closed};
    globctx.bind(event, "terminate");
    event.set_type(sf::Event::KeyPressed);
    event.set_key(sf::Keyboard::Escape);
    globctx.bind(event, "terminate");

    event.set_key(sf::Keyboard::Q);
    globctx.bind(event, [&](){ running = false; });

    event.set_key(sf::Keyboard::P);
    globctx.bind(event, [&](){ update_pause = !update_pause; });

    event.set_key(sf::Keyboard::S);
    globctx.bind(event, [&](){
        save();
        return true;
    });

    event.set_key(sf::Keyboard::L);
    globctx.bind(event, [&](){
        load();
        return true;
    });

    input::Event follow{sf::Event::KeyPressed};
    follow.set_key(sf::Keyboard::F);
    follow.set_mod(input::Mod::CTRL);
    globctx.bind(follow, [&](const input::Event &){
        Entity * entity = nullptr;
        selectm.map([&](Entity & e){ entity = &e; });
        if (entity == nullptr) {
            signals.update.remove_callback("follow");
            return false;
        }
        signals.update.add_callback("follow", [&,entity](float){
            camera.focus_at(entity->get_position());
        });
        return true;
    });

    input::Event zoom{sf::Event::MouseWheelScrolled};
    globctx.bind(zoom, [&](const input::Event & event){
        if (inputm.is_button_pressed(sf::Mouse::Middle) == true) {
            return false;
        }
        if (event.get_scroll() > 0) {
            camera.zoom(config::zoomfactor);
        }
        else {
            camera.zoom(1/config::zoomfactor);
        }
        return true;
    });

    input::Event move_world{sf::Event::MouseMoved};
    globctx.bind(move_world, [&](const input::Event & event){
        if (inputm.is_button_pressed(sf::Mouse::Middle)) {
            // note: mousedt is the actual change, and so moving the camera by
            // that amount makes it look like the world is scrolling in the 
            // opposite direction, multiply by -1 to make it look like we are
            // moving the world instead of the camera 
            camera.scroll(sf::Vector2f(-1 * event.get_mousedt()));
            return true;
        }
        return false;
    });
}

Engine::Engine(SFML & sf)
    :sfml(sf),
     camera(sfml.window),
     inputm(sfml.window),
     wrender(sfml.texture),
     uirender(sfml.texture),
     spritem(),
     spritef(spritem, wrender),
     anims(),
     animf(anims, spritef, wrender),
     animm(),
     entityf(animf, wrender),
     entitym(entityf),
     moves(),
     tilef(spritef, wrender),
     tilem(tilef),
     pathm(moves),
     selectm(spritef, entitym)
{
}

void Engine::poll_events() {
    inputm.poll_sfevents();
}

void Engine::draw(const sf::Color & bgcolor) {
    sfml.window.clear(bgcolor);
    wrender.draw(sfml.window);
    uirender.draw(sfml.window);
    sfml.window.display();
}

void Engine::update() {
    entitym.update(16);
    selectm.system_update(16);
    pathm.system_update(16);
    moves.system_update(16);
    anims.update(16);
    signals.update(16);
}

void Engine::run() {
    running = true;
    while (running) {
        poll_events();
        if (update_pause == false) {
            update();
        }
        draw();
    }
}

void Engine::stop() {
    running = false;
}

void Engine::reset() {
    tilem.clear();
    entitym.clear();
    camera.focus_at({0, 0});
}

void Engine::load(const std::string & filename, const std::string & path) {
    try {
        IOReader in{path + filename};
        try {

            camera.deserialize(in);
            tilem.deserialize(in);
            pathm.init(tilem.generate_graph());
            entitym.deserialize(in);

        }
        catch (std::invalid_argument) {
            this->reset();
        }
    }
    catch (std::invalid_argument) {
        std::cerr << "Unable to load " << filename << std::endl;
    }
}

const Position::Region world_size(TileManager & tilem) {
    float right = std::nanf("0"), top = std::nanf("0"), left = std::nanf("0"), bot = std::nanf("0");
    tilem.map([&](Tile & tile){
        const auto region = tile.get_region();
        const auto _top = region.top_left().to_pixel().y;
        const auto _left = region.bottom_left().to_pixel().x;
        const auto _right = region.top_right().to_pixel().x;
        const auto _bottom = region.bottom_right().to_pixel().y;
        left  = std::fmin(_left, left);  
        top   = std::fmin(_top, top);
        right = std::fmax(_right, right); 
        bot   = std::fmax(_bottom, bot);
    });
    return Position::Region{left, top, right - left, bot - top};
}

void Engine::save(const std::string & filename, const std::string & path) {
    // TODO
    // One of these failing results in partial save, corrupting the archive.
    // Create atomic write-to-file.
    IOWriter out{path + filename};
    try {
        camera.serialize(out);
        tilem.serialize(out);
        entitym.serialize(out);
    }
    catch (std::invalid_argument) {
        std::cerr << "Unable to save " << filename << std::endl;
    }

    sf::View view;
    auto region = world_size(tilem);
    view.setSize(region.width, region.height);
    view.setCenter({region.x+region.width/2, region.y+region.height/2});

    sf::RenderWindow window{sf::VideoMode(config::minimap_width, config::minimap_height), ""};
    window.setView(view);
    sf::Texture texture;
    texture.create(config::minimap_width, config::minimap_height);
    WorldRender rs{sfml.texture};
    std::list<Sprite> tile_sprites;
    tilem.map([&](Tile & tile){
        tile_sprites.push_back(spritef.copy(rs, tile.sprite));
    });
    window.clear(sf::Color::Black);
    rs.draw(window);
    window.display();
    texture.update(window);

    sf::Image img = texture.copyToImage();
    const sf::Vector2u img_size = texture.getSize();
    //const auto pixels = reinterpret_cast<const std::uint32_t *>(img.getPixelsPtr());
    auto pixels = img.getPixelsPtr();
    const std::size_t array_size = img_size.x * img_size.y;
    assert(array_size == config::minimap_width * config::minimap_height);
    sfml.texture.update(pixels, 256, 256, 0, 0);
    img.saveToFile("../sprites/" + filename + ".png");
}
