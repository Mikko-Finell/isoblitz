#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "ui.hpp"

#define WINW 1800
#define WINH 1000
#define TILEW 128.0f
#define TILEH 64.0f
#define HALFW (TILEW / 2)
#define HALFH (TILEH / 2)

sf::Vector2f logic_to_pixel(const sf::Vector2f & v) {
    float x = (v.x - v.y) * HALFW;
    float y = (v.x + v.y) * HALFH;
    return {x, y};
}

sf::Vector2f pixel_to_logic(const sf::Vector2f & v) {
    float x = (v.x / HALFW + v.y / HALFH) / 2;
    float y = (v.y / HALFH - v.x / HALFW) / 2;
    return {x, y};
}

class Tile {
    sf::Vector2f coordinate;

public:
    Tile() {}
    Tile(const sf::Vector2f & coord) : coordinate(coord) {}

    bool operator==(const Tile & t) const {
	return t.coordinate == coordinate;
    }

    bool operator<(const Tile & t) const {
	if (t.coordinate.y == coordinate.y) {
	    return t.coordinate.x > coordinate.x;
	}
	else {
	    return t.coordinate.y > coordinate.y;
	}
    }

    void draw(std::vector<sf::Vertex> & vertices) const {
        static float sprite_x = 0;
        static float sprite_y = 0;
	static float sprite_w = TILEW;
	static float sprite_h = TILEH * 2;

        auto pixel_pos = logic_to_pixel(coordinate);
	auto offset = sprite_h / 4;

        auto topleft = pixel_pos + sf::Vector2f{0, -offset};
        auto topright = pixel_pos + sf::Vector2f{TILEW, -offset};
        auto botright = pixel_pos + sf::Vector2f{TILEW, sprite_h - offset};
        auto botleft = pixel_pos + sf::Vector2f{0, sprite_h - offset};

        // sprite coords
        sf::Vector2f spr_xy{sprite_x, sprite_y};
        auto c_topleft = spr_xy + sf::Vector2f{0, 0};
        auto c_topright = spr_xy + sf::Vector2f{sprite_w, 0};
        auto c_botright = spr_xy + sf::Vector2f{sprite_w, sprite_h};
        auto c_botleft = spr_xy + sf::Vector2f{0, sprite_h};

        vertices.push_back(sf::Vertex{topleft, c_topleft});
        vertices.push_back(sf::Vertex{topright, c_topright});
        vertices.push_back(sf::Vertex{botright, c_botright});
        vertices.push_back(sf::Vertex{botleft, c_botleft});
    }
};

sf::Vector2f snap_to_grid(const sf::Vector2f & v) {
    auto w = pixel_to_logic(v);
    w.x = floor(w.x);
    w.y = floor(w.y);
    return logic_to_pixel(w);
}

sf::Vector2f tile_center_at(sf::Vector2f v) {
    v.x -= HALFW;
    //v.y -= HALFH;
    auto w = snap_to_grid(v);
    return pixel_to_logic(w);
}

void user_click(sf::Vector2f v, std::vector<Tile> & tiles) {
    tiles.push_back(Tile{tile_center_at(v)});
}

int main() {
    sf::RenderWindow window{sf::VideoMode{WINW, WINH}, "Bullet Editor"};
    sf::Texture spritesheet{};
    spritesheet.loadFromFile("sprites128x64.png");
    //window.setMouseCursorVisible(false);
    //view.setSize(WINW/2, WINH/2);
    //window.setView(view);

START:
    UI ui;
    ui.attach(window);
    
    std::vector<Tile> tiles;
    std::vector<sf::Vertex> vertices;
    bool mouse_pressed = false;

    while (window.isOpen()) {
	auto cmds = ui.handle_events();
	auto dt = ui.mouse_dt();

	if (ui.is_mouse_pressed()) {
	    user_click(ui.mouse_pos(), tiles);
	    std::sort(tiles.begin(), tiles.end());
	}

	for (auto cmd : cmds) {
	    if (cmd == CMD::Restart) {
		goto START;
	    }
	    else if (cmd == CMD::Quit) {
		window.close();
		break;
	    }
	    else if (cmd == CMD::Scroll) {
		auto view = window.getView();
		auto dt = ui.mouse_dt();
		view.move(dt);
		window.setView(view);
		/*
		*/
	    }
	}

	vertices.clear();
        for (auto & tile : tiles) {
            tile.draw(vertices);
        }

	auto tmptile = Tile{tile_center_at(ui.mouse_pos())};
	tmptile.draw(vertices);
        
	window.clear(sf::Color::White);
        window.draw(&vertices[0], vertices.size(), sf::Quads, &spritesheet);
	window.display();
    }
}
