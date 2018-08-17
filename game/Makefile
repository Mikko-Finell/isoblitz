# game Makefile

CMNDIR := ../common
CC := clang++
CFLAGS := -std=c++17 -Wfatal-errors -I.. 
EXE := game.out
LIBS := -lsfml-system -lsfml-window -lsfml-graphics -lsqlite3
LINKTYPE := static

cmnheaders := $(wildcard $(CMNDIR)/*.hpp)

OBJS := entity.o \
    	entityfactory.o \
	game.o \
	hitbox.o \
	map.o \
	selection.o \
	tile.o \

all: $(EXE)

ifeq ($(LINKTYPE),static)
$(EXE): $(OBJS) $(CMNDIR)/engine.a
	$(CC) $(CFLAGS) $(LIBS) -L$(CMNDIR) $^ -o $@

$(CMNDIR)/engine.a: $(wildcard $(CMNDIR)/*.cpp) $(wildcard $(CMNDIR)/*.hpp)
	make static -C $(CMNDIR)
else
$(EXE): $(OBJS) $(CMNDIR)/libengine.so
	$(CC) $(CFLAGS) -Wl,-rpath=$(CMNDIR) -L $(CMNDIR) $(LIBS) -lengine $^ -o $@

$(CMNDIR)/libengine.so: $(wildcard $(CMNDIR)/*.cpp) $(wildcard $(CMNDIR)/*.hpp)
	make dynamic -C $(CMDNIR)
endif

entity.o: %.o: %.cpp
	$(CC) $(CFLAGS) -c $<

entityfactory.o: %.o: %.cpp
	$(CC) $(CFLAGS) -c $<
	
game.o: %.o: %.cpp 
	$(CC) $(CFLAGS) -c $<

hitbox.o: %.o: %.cpp 
	$(CC) $(CFLAGS) -c $<

map.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

selection.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

tile.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(EXE) $(wildcard *.o)
