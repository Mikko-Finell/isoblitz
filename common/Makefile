# common Makefile

CC := clang++
CFLAGS := -fPIC -std=c++17 -Wfatal-errors -g

common := animation.o \
	input.o \
	mapobject.o \
	observer.o \
	sprite.o

headeronly := timer.hpp.gch \
	helper.hpp.gch \
	serializable.hpp.gch \

all: libengine.so

libengine.so: $(common) $(headeronly)
	$(CC) -shared $(common) -o libengine.so

animation.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

helper.hpp.gch: helper.hpp
	$(CC) $(CFLAGS) -c helper.hpp

input.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

mapobject.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

observer.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

serializable.hpp.gch: %.gch: %
	$(CC) $(CFLAGS) -c $<

sprite.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

timer.hpp.gch: %.gch: %
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(wildcard *.o *.so *.gch)
