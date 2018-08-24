#include "rendersystem.hpp"
#include "sprite.hpp"
#include <iostream>

RenderSystem::RenderSystem(sf::Texture & tex) : texture(tex) {
}

void RenderSystem::remove(GameObject * go) {
    Sprite * sprite = dynamic_cast<Sprite *>(go);
    assert(sprite);
    sprites.erase(sprite);
    go->reg(nullptr);
}

bool RenderSystem::add(Sprite * sprite) {
    if (sprites.insert(sprite).second) {
        sprite->reg(this);
        return true;
    }
    return false;
}

void RenderSystem::unlist(Sprite * sprite) {
    sprites.erase(sprite);
}

// WorldRender //////////////////////////////////////////////////////////////////

void WorldRender::draw(sf::RenderWindow & window) {
    auto view = window.getView();
    auto center = sf::Vector2i(view.getCenter());
    auto size = sf::Vector2i(view.getSize());
    auto pos = sf::Vector2i(center - size / 2);

    sf::IntRect screen{pos, size};
    static std::vector<Sprite *> visible_sprites;
    visible_sprites.clear();
    for (auto & sprite : sprites) {
        if (sprite->get_screencoords().intersects(screen)) {
            visible_sprites.push_back(sprite);
        }
    }
    auto cmp = [this](const Sprite * lhs, const Sprite * rhs){
        return *lhs < *rhs;
    };
    std::sort(visible_sprites.begin(), visible_sprites.end(), cmp);

    static std::vector<sf::Vertex> vs;
    if (vs.size() < 4 * visible_sprites.size()) {
        vs.resize(4 * visible_sprites.size());
    }

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

bool UIRender::add(Sprite * sprite) {
    if (RenderSystem::add(sprite)) {
        sorted_sprites.push_back(sprite);
        auto cmp = [](const Sprite * lhs, const Sprite * rhs){
            return lhs->get_layer() < rhs->get_layer();
        };
        std::sort(sorted_sprites.begin(), sorted_sprites.end(), cmp);
        return true;
    }
    return false;
}

void UIRender::remove(GameObject * go) {
    auto sprite = dynamic_cast<Sprite *>(go);
    assert(sprite);

    unlist(sprite);
    go->reg(nullptr);
}

void UIRender::unlist(Sprite * sprite) {
    if (sprites.erase(sprite)) {
        auto itr = std::find(sorted_sprites.begin(), sorted_sprites.end(), 
                             sprite);
        assert(itr != sorted_sprites.end());
        *itr = sorted_sprites.back();
        sorted_sprites.pop_back();
    }
}

void UIRender::draw(sf::RenderWindow & window) {
    static std::vector<sf::Vertex> vs;
    const auto vertex_count = 4 * sorted_sprites.size();
    vs.resize(vertex_count);
    std::size_t idx = 0;
    for (auto itr = sorted_sprites.begin(); idx < vertex_count; idx += 4) {
        Sprite * sprite = *itr;
        sprite->draw(&vs[idx]);
        ++itr;
    }

    auto view = window.getView();
    window.setView(window.getDefaultView());
    window.draw(&vs[0], idx, sf::Quads, &texture);
    window.setView(view);
}
