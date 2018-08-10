#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "shell.hpp"
#include "common/repl.hpp"

static Shell * shell;

void thread_fn(Shell & shell) {
    /*
        else if (keyword == "name") {
            if (tokens.size() != 2) {
                goto ERROR;
            }
            std::string str = tokens.at(1);
            shell.defer([str,&shell](){ shell.signal.set_mapname(str); });
        }
        else if (keyword == "bgcolor") {
            if (tokens.size() != 4) { // color r g b
                goto ERROR;
            }
            sf::Color c;
            try {
                c.r = std::stoi(tokens.at(1));
                c.g = std::stoi(tokens.at(2));
                c.b = std::stoi(tokens.at(3));
            }
            catch (...) {
                goto ERROR;
            }
            shell.defer([c,&shell](){ shell.signal.set_bgcolor(c); });
        }
        else {
            goto ERROR;
        }
	continue;
ERROR:
	std::cout << "Invalid argument: ";
	for (auto & t : tokens) {
	    std::cout << t << ", ";
	}
	std::cout << std::endl;
    }
    std::cout << std::endl;
    */
}

void Shell::defer(const std::function<void()> & event) {
    std::lock_guard<std::mutex> lock{mutex};
    deferred_signals.push_back(event);
}

void Shell::emit_signals() {
    std::lock_guard<std::mutex> lock{mutex};
    for (auto & signal : deferred_signals) {
        signal();
    }
    deferred_signals.clear();
}

static s7_pointer quit(s7_scheme * sc, s7_pointer args) {
    shell->defer([](){ shell->signal.quit(); });
    s7_quit(sc);
    return s7_nil(sc);
}

static s7_pointer block(s7_scheme * sc, s7_pointer args) {
    bool b;
    if (s7_is_boolean(s7_car(args)) == false) {
        return s7_wrong_type_arg_error(sc, "block", 1, s7_car(args), "a bool");
    }
    else {
        b = s7_boolean(sc, (s7_car(args)));
        shell->defer([b](){ shell->signal.set_blocked(b); });
    }

    return s7_nil(sc);
}

static s7_pointer tile(s7_scheme * sc, s7_pointer args) {
    int x, y;
    s7_pointer first = s7_car(args);
    s7_pointer second = s7_car(s7_cdr(args));
    if (s7_is_integer(first)) {
        x = s7_integer(first);
    }
    else {
        return s7_wrong_type_arg_error(sc, "tile", 1, first, "an integer");
    }
    if (s7_is_integer(second)) {
        y = s7_integer(second);
        // x and y ok, create signal
        shell->defer([x,y](){
            shell->signal.set_sprite(sf::Vector2i{x, y});
        });
    }
    else {
        return s7_wrong_type_arg_error(sc, "tile", 2, second, "an integer");
    }
    return s7_nil(sc);
}

static s7_pointer newmap(s7_scheme * sc, s7_pointer args) {
    if (s7_is_string(s7_car(args))) {
        const std::string str = s7_string(s7_car(args));
        shell->defer([str](){ shell->signal.newmap(str); });
    }
    else {
        return s7_wrong_type_arg_error(sc, "newmap", 1, s7_car(args), "a string");
    }
    return s7_nil(sc);
}

static s7_pointer save(s7_scheme * sc, s7_pointer args) {
    std::string str = "";
    if (s7_list_length(sc, args) == 0) {
        // do nothing
    }
    else if (s7_is_string(s7_car(args))) {
        str = s7_string(s7_car(args));
    }
    else {
        return s7_wrong_type_arg_error(sc, "save", 1, s7_car(args), "a string");
    }
    shell->defer([str](){ shell->signal.save(str); });
    return s7_nil(sc);
}

static s7_pointer load(s7_scheme * sc, s7_pointer args) {
    std::string str = "";
    if (s7_list_length(sc, args) == 0) {
        // do nothing
    }
    else if (s7_is_string(s7_car(args))) {
        str = s7_string(s7_car(args));
    }
    else {
        return s7_wrong_type_arg_error(sc, "load", 1, s7_car(args), "a string");
    }
    shell->defer([str](){ shell->signal.load(str); });
    return s7_nil(sc);
}

static s7_pointer name(s7_scheme * sc, s7_pointer args) {
    if (s7_is_string(s7_car(args))) {
        const std::string str = s7_string(s7_car(args));
        shell->defer([str](){ shell->signal.set_mapname(str); });
    }
    else {
        return s7_wrong_type_arg_error(sc, "name", 1, s7_car(args), "a string");
    }
    return s7_nil(sc);
}

static s7_pointer bgcolor(s7_scheme * sc, s7_pointer args) {
    if (s7_list_length(sc, args) < 3) {
        return s7_wrong_number_of_args_error(sc, "bgcolor", args);
    }
    sf::Uint8 r, g, b;
    s7_pointer first = s7_car(args);
    s7_pointer second = s7_car(s7_cdr(args));
    s7_pointer third = s7_car(s7_cdr(s7_cdr(args)));

    if (s7_is_integer(first)) {
        r = s7_integer(first);

        if (s7_is_integer(second)) {
            g = s7_integer(second);

            if (s7_is_integer(third)) {
                b = s7_integer(third);

                sf::Color color{r, g, b};
                shell->defer([color](){ shell->signal.set_bgcolor(color); });

                return s7_nil(sc);
            }
        }
    }

    return s7_wrong_type_arg_error(sc, "bgcolor", 1, s7_car(args), "an int");
}

static s7_pointer paint(s7_scheme * sc, s7_pointer args) {
    int x, y;
    s7_pointer first = s7_car(args);
    s7_pointer second = s7_car(s7_cdr(args));
    if (s7_is_integer(first)) {
        x = s7_integer(first);
    }
    else {
        return s7_wrong_type_arg_error(sc, "paint", 1, first, "an integer");
    }
    if (s7_is_integer(second)) {
        y = s7_integer(second);
        // x and y ok, create signal
        shell->defer([x,y](){
            shell->signal.paint(sf::Vector2f(x, y));
        });
    }
    else {
        return s7_wrong_type_arg_error(sc, "paint", 2, second, "an integer");
    }
    return s7_nil(sc);
}

void Shell::launch() {
    shell = this;
    std::thread thread{[](){
        REPL repl;
        repl.init();
        repl.bind("quit", quit, 0, 0, false, "");
        repl.bind("tile", tile, 2, 0, false, "(tile int int) sets tile spritecoords");
        repl.bind("block", block, 1, 0, false, "(block bool) sets tile blocked");
        repl.bind("newmap", newmap, 1, 0, false, "(newmap string) opens new map");
        repl.bind("save", save, 0, 1, false, "(save string) saves current map with name=string");
        repl.bind("load", load, 0, 1, false, "(load string) loads map with name=string");
        repl.bind("name", name, 1, 0, false, "(name string) sets name of current map");
        repl.bind("bgcolor", bgcolor, 3, 0, false, "(bgcolor, int,int,int)");
        repl.bind("paint", paint, 2, 0, false, "(paint int int) creates a tile at param");
        repl.eval(R"(
          (define (fill x y c r)
            (cond ((= r 0) 0)
              (else
                (define (paint-row x y n)
                  (cond ((= n 0) 0)
                    (else (paint x y)
                      (paint-row (+ x 1) y (- n 1)))))
                (paint-row x y c)
                (fill x (+ y 1) c (- r 1)))))
        )");
        repl.run();
    }};
    thread.detach();
}

Shell::~Shell() {
}
