#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "shell.hpp"
#include "coordinate.hpp"

void thread_fn(Shell & shell) {
    std::string buffer;
    std::vector<std::string> tokens;
    while (1) {
	std::cout << "> ";
	std::getline(std::cin, buffer);

	std::stringstream ss;
	ss.str(buffer);

	tokens.clear();
	while (ss >> buffer) {
	    tokens.push_back(buffer);
	}
	if (tokens.empty()) {
	    continue;
	}
	auto keyword = tokens.at(0);
	if (keyword == "quit") {
	    shell.store_event(Event::Quit);
	    break;
	}
	else if (keyword == "tile") {
	    int x, y;
	    try {
		x = std::stoi(tokens.at(1));
		y = std::stoi(tokens.at(2));
		Event event{Event::SetSprite};
                event.param = Coordinate(x, y);
		shell.store_event(event);
	    }
	    catch (...) {
		goto ERROR;
	    }
	}
        else if (keyword == "save") {
            Event save{Event::Save};
            if (tokens.size() == 2) {
                save.param = tokens.at(1);
            }
            else if (tokens.size() != 1) {
                goto ERROR;
            }
            shell.store_event(save);
        }
        else if (keyword == "load") {
            Event load{Event::Load};
            if (tokens.size() == 2) {
                load.param = tokens.at(1);
            }
            else if (tokens.size() != 1) {
                goto ERROR;
            }
            shell.store_event(load);
        }
        else if (keyword == "new") {
            Event newmap{Event::New};
            if (tokens.size() == 2) {
                newmap.param = tokens.at(1);
            }
            else if (tokens.size() != 1) {
                goto ERROR;
            }
            shell.store_event(newmap);
        }
        else if (keyword == "name") {
            if (tokens.size() != 2) {
                goto ERROR;
            }
            Event name{Event::SetMapName};
            name.param = tokens.at(1);
            shell.store_event(name);
        }
        else if (keyword == "spritesheet") {
            if (tokens.size() != 2) {
                goto ERROR;
            }
            Event event{Event::SetSpriteSheet};
            event.param = tokens.at(1);
            /* TODO
            shell.store_event(event);
            */
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
}

void Shell::store_event(const Event & event) {
    std::lock_guard<std::mutex> lock{mutex};
    events.push_back(event);
}

void Shell::emit_events() {
    std::lock_guard<std::mutex> lock{mutex};
    for (auto & event : events) {
	emit(event);
    }
    events.clear();
}

void Shell::launch() {
    thread = std::thread{thread_fn, std::ref(*this)};
    thread.detach();
}

Shell::~Shell() {
}
