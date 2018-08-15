# common Makefile

CC := clang++
CFLAGS := -fPIC -std=c++17 -Wfatal-errors -g

common := animation.o \
	camera.o \
	input.o \
	mapobject.o \
	observer.o \
	sprite.o 

headeronly := util.hpp.gch

static: engine.a

dynamic: libengine.so

all: engine.a

libengine.so: $(common) $(headeronly)
	$(CC) -shared $(common) -o libengine.so
	
engine.a: $(common) $(headeronly)
	ar rcs $@ $^

animation.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

camera.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

util.hpp.gch: util.hpp
	$(CC) $(CFLAGS) -c util.hpp

input.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

mapobject.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

observer.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

sprite.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(wildcard *.o *.a *.so *.gch)
