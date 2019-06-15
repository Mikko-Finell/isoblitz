#include "rendersystem.hpp"
#include "sprite.hpp"
#include <iostream>
#include <cassert>

RenderSystem::RenderSystem(sf::Texture & tex) : texture(tex) {
}

bool RenderSystem::add(Sprite * sprite, const std::string & caller) {
    //std::cout << "RenderSystem: Sprite added by " << caller << std::endl;
    if (sprites.insert(sprite).second) {
        return true;
    }
    return false;
}

void RenderSystem::unlist(Sprite * sprite) {
    assert(sprites.erase(sprite) == 1);
}

// WorldRender //////////////////////////////////////////////////////////////////

void WorldRender::remove(Sprite & sprite) {
    assert(sprites.erase(&sprite) == 1);
}

void WorldRender::draw(sf::RenderWindow & window) {
    const auto view = window.getView();
    const auto center = sf::Vector2f(view.getCenter());
    const auto size = sf::Vector2f(view.getSize());
    const auto pos = sf::Vector2f(center - size / 2.0f);

    // collect all sprites whose screencoords intersect with the window
    // TODO hard nicetohave
    // the cost of this is O(n) for number of sprites; Create spatial 
    // partitioning system to replace this culling mechanism.
    sf::FloatRect screen{pos, size};
    static std::vector<Sprite *> visible_sprites;
    visible_sprites.clear();
    for (auto & sprite : sprites) {
        if (sprite->get_screencoords().intersects(screen)) {
            visible_sprites.push_back(sprite);
        }
    }

    // isometric sort using sprites own ordering method
    // TODO easy critical
    // this sorts based on spritecoords instead of origin which results in wrong order
    auto cmp = [this](const Sprite * lhs, const Sprite * rhs){
        return *lhs < *rhs;
    };
    // cppreference says this is O(n logn)
    std::sort(visible_sprites.begin(), visible_sprites.end(), cmp);

    static std::vector<sf::Vertex> vs;

    // resize only in the case that we need more space
    if (vs.size() < 4 * visible_sprites.size()) {
        vs.resize(4 * visible_sprites.size());
    }

    // add the vertices for every visible sprite then draw all vertices; O(2n)
    std::size_t idx = 0;
    const auto vertex_count = visible_sprites.size() * 4;
    for (auto itr = visible_sprites.begin(); idx < vertex_count; idx += 4) {
        Sprite * sprite = *itr;
        sprite->draw(&vs[idx]);
        ++itr;
    }
    window.draw(&vs[0], idx, sf::Quads, &texture);
}

// UIRender /////////////////////////////////////////////////////////////////////

bool UIRender::add(Sprite * sprite, const std::string & caller) {
    if (RenderSystem::add(sprite, caller + " (via subclass UIRender)")) {
        sorted_sprites.push_back(sprite);
        sorted = false;
        return true;
    }
    return false;
}

void UIRender::unlist(Sprite * sprite) {
    // sprites is unordered_set so erase is O(1) on average
    if (sprites.erase(sprite)) {
        auto itr = std::find(sorted_sprites.begin(), sorted_sprites.end(), 
                             sprite);
        assert(itr != sorted_sprites.end());
        *itr = sorted_sprites.back();
        sorted_sprites.pop_back();

        // Sort is necessary because we do fast-erase which changes the ordering.
        // This is faster when unlisting multiple sprites between draw, because
        // each call to Vector::erase is O(n), but one call to sort is O(n logn)
        // and in all likelihood we seldom unlist a single sprite from UI.
        sorted = false;
    }
}

void UIRender::draw(sf::RenderWindow & window) {
    // if sprites were added or removed then sort them;
    if (sorted == false) {
        auto cmp = [](const Sprite * lhs, const Sprite * rhs){
            return lhs->get_layer() < rhs->get_layer();
        };
        std::sort(sorted_sprites.begin(), sorted_sprites.end(), cmp);
        sorted = true;
    }
    static std::vector<sf::Vertex> vs;
    const auto vertex_count = 4 * sorted_sprites.size();
    vs.resize(vertex_count);
    std::size_t idx = 0;
    for (auto itr = sorted_sprites.begin(); idx < vertex_count; idx += 4) {
        Sprite * sprite = *itr;
        sprite->draw(&vs[idx]);
        ++itr;
    }

    // this causes the UI sprites to be drawn correctly in a stationary location
    // on the window regardless or zoom or camera movement
    auto view = window.getView();
    window.setView(window.getDefaultView());
    window.draw(&vs[0], idx, sf::Quads, &texture);
    window.setView(view);
}
