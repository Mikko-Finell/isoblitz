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
	    shell.store_event(Event{Event::Quit});
	    break;
	}
	else if (keyword == "tile") {
	    int x, y;
	    try {
		x = std::stoi(tokens.at(1));
		y = std::stoi(tokens.at(2));
		Event event{Event::SetTileType};
		event.coordinate = Coordinate(x, y);
		shell.store_event(event);
	    }
	    catch (...) {
		goto ERROR;
	    }
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

void Shell::store_event(Event event) {
    std::lock_guard<std::mutex> lock{mutex};
    events.push_back(event);
}

void Shell::emit_events() {
    std::lock_guard<std::mutex> lock{mutex};
    for (auto event : events) {
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
