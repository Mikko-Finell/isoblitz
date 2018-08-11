# game Makefile

CMNDIR := ../common
CC := clang++
CFLAGS := -std=c++17 -Wfatal-errors -I.. 
EXE := game.out
LIBS := -lsfml-system -lsfml-window -lsfml-graphics -ldl -lsqlite3 -lengine

OBJS := game.o \
	map.o \
	tile.o \

all: $(EXE)

$(EXE): $(OBJS) $(CMNDIR)/libengine.so
	$(CC) $(CFLAGS) -Wl,-rpath=$(CMNDIR) -L $(CMNDIR) $(LIBS) -o $@ $(OBJS)

game.o: %.o: %.cpp
	$(CC) $(CFLAGS) -c $<

map.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

tile.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

$(CMNDIR)/libengine.so: $(wildcard $(CMNDIR)/*.cpp) $(wildcard $(CMNDIR)/*.hpp)
	make -C ../common

clean:
	rm -rf $(EXE) $(wildcard *.o)
