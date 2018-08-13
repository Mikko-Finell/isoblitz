# game Makefile

CMNDIR := ../common
CC := clang++
CFLAGS := -std=c++17 -Wfatal-errors -I.. 
EXE := game.out
LIBS := -lsfml-system -lsfml-window -lsfml-graphics -lsqlite3
LINKTYPE := static

cmnheaders := $(wildcard $(CMNDIR)/*.hpp)

OBJS := game.o \
	map.o \
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

game.o: %.o: %.cpp $(cmnheaders)
	$(CC) $(CFLAGS) -c $<

map.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

tile.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(EXE) $(wildcard *.o)
