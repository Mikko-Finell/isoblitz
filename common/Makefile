# common Makefile

CC := clang++
CFLAGS := -fPIC -std=c++17 -Wfatal-errors -g

common := animation.o \
    	animationfactory.o \
	camera.o \
	database.o \
	input.o \
	observer.o \
	rendersystem.o \
	spritefactory.o \
	sprite.o \
	tile.o \
	tilefactory.o

headeronly := util.hpp.gch
allheaders := $(wildcard *.hpp)

static: engine.a

dynamic: libengine.so

all: engine.a

libengine.so: $(common) $(allheaders)
	$(CC) -shared $(common) -o libengine.so
	
engine.a: $(common) $(allheaders)
	ar rcs $@ $^

animation.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

animationfactory.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

camera.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

database.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

util.hpp.gch: util.hpp
	$(CC) $(CFLAGS) -c util.hpp

input.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

mapobject.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

observer.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

rendersystem.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

spritefactory.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

sprite.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

tile.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

tilefactory.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(wildcard *.o *.a *.so *.gch)
