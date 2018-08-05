#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "common/coordinate.hpp"
#include "shell.hpp"

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
            shell.defer([&shell](){ shell.signal.quit(); });
	    break;
	}
	else if (keyword == "tile") {
	    float x, y;
	    try {
		x = std::stoi(tokens.at(1));
		y = std::stoi(tokens.at(2));
                shell.defer([x,y,&shell](){
                    shell.signal.set_sprite(Coordinate{x, y});
                });
	    }
	    catch (...) {
		goto ERROR;
	    }
	}
        else if (keyword == "block") {
            bool b = false;
            if (tokens.size() == 2) {
                auto v = tokens.at(1);
                if (v == "true" || v == "t" || v == "1") {
                    b = true;
                }
                else if (v == "false" || v == "f" || v == "0") {
                    b = false;
                }
                else {
                    goto ERROR;
                }
            }
            else {
                goto ERROR;
            }
            shell.defer([b,&shell](){ shell.signal.set_blocked(b); });
        }
        else if (keyword == "save") {
            if (tokens.size() == 2) {
                const std::string mapname = tokens.at(1);
                shell.defer([mapname,&shell](){
                    shell.signal.set_mapname(mapname);
                    shell.signal.save("");
                });
            }
            else if (tokens.size() != 1) {
                goto ERROR;
            }
            shell.defer([&shell](){ shell.signal.save(""); });
        }
        else if (keyword == "load") {
            std::string str = "";
            if (tokens.size() == 2) {
                str = tokens.at(1);
            }
            else if (tokens.size() != 1) {
                goto ERROR;
            }
            shell.defer([str,&shell](){ shell.signal.load(str); });
        }
        else if (keyword == "new") {
            std::string str = "";
            if (tokens.size() == 2) {
                str = tokens.at(1);
            }
            else if (tokens.size() != 1) {
                goto ERROR;
            }
            shell.defer([str,&shell](){ shell.signal.newmap(str); });
        }
        else if (keyword == "name") {
            if (tokens.size() != 2) {
                goto ERROR;
            }
            std::string str = tokens.at(1);
            shell.defer([str,&shell](){ shell.signal.set_mapname(str); });
        }
        else if (keyword == "spritesheet") {
            /* TODO
            if (tokens.size() != 2) {
                goto ERROR;
            }
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

void Shell::launch() {
    thread = std::thread{thread_fn, std::ref(*this)};
    thread.detach();
}

Shell::~Shell() {
}
