# IsoBlitz - Isometric realtime strategy game engine

IsoBlitz is a framework for creating isometric rts games like Starcraft Broodwar, and Age Of Empires. IsoBlitz is meant to be simple, modular, and easily extendable. The design principle is to optimize for robustness and code-readability. The following tactics are utilized for moving towards that goal:
* Create good documentation
* Enforce code contracts
* High assertion density
* Use `const` variables and pure functions

## License

Public domain. My intent is that any code or ideas you find here are 
truly and fully yours for any purpose in perpetuity.

## Dependencies

C++17 or above.

[SFML](https://github.com/SFML/SFML)

[SQLite3](https://sqlite.org/download.html)

## Installation

1. Clone or download this repo
2. Build and install the eninge: `make -C common`
3. Build and install the game: `make -C game`
4. Build and install the map editor: `make -C editor`
