# common Makefile

CC := clang++
CFLAGS := -fPIC -std=c++17 -Wfatal-errors -g
common := animation.o \
	animationfactory.o \
	camera.o \
	coordinate.o \
	database.o \
	engine.o \
	entity.o \
	entityfactory.o \
	entitymanager.o \
	entitysystem.o \
	graph.o \
	hitbox.o \
	input.o \
	movementsystem.o \
	pathmanager.o \
	rendersystem.o \
	state.o \
	serialize.o \
	selectionmanager.o \
	spritemanager.o \
	spritefactory.o \
	sprite.o \
	tile.o \
	tilefactory.o \
	tilemanager.o \
	ui.o

headeronly := util.hpp.gch config.hpp.gch
allheaders := $(wildcard *.hpp) $(headeronly)

all: dynamic

static: engine.a

dynamic: libengine.so

libengine.so: $(common) $(allheaders)
	$(CC) -shared $(common) -o libengine.so
	
engine.a: $(common) $(allheaders)
	ar rcs $@ $^

config.hpp.gch: config.hpp
	$(CC) $(CFLAGS) -c config.hpp

util.hpp.gch: util.hpp
	$(CC) $(CFLAGS) -c util.hpp

animation.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

animationfactory.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

camera.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

coordinate.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

database.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

engine.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

entity.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

entityfactory.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

entitymanager.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

entitysystem.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

graph.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

hitbox.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

input.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

movementsystem.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

pathmanager.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

rendersystem.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

state.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

serialize.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

selectionmanager.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

spritemanager.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

spritefactory.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

sprite.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

tile.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

tilefactory.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

tilemanager.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

ui.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(wildcard *.o *.a *.so *.gch)
