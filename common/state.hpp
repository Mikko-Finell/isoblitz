#ifndef state_hpp
#define state_hpp

#include "engine.hpp"
#include "sfml.hpp"
#include <unordered_map>
#include <string>
#include <memory>

class StateManager {
    std::unordered_map<std::string, std::unique_ptr<Engine>> states;
    SFML sfml;

    StateManager();
    ~StateManager();
	StateManager(const StateManager &);
	const StateManager & operator=(const StateManager &);

public:
    static StateManager & get_manager();
    static Engine & create(const std::string & name);
    static Engine & get_state(const std::string & name);
    static void run(const std::string & name);
    static void terminate();
};

#endif
