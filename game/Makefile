# game Makefile

CC := clang++
CFLAGS := -std=c++17 -Wfatal-errors -g -I..
EXE := game.out
LIBS := -lsfml-system -lsfml-window -lsfml-graphics -lsqlite3
CMNDIR := ../common
LINKTYPE := dynamic
cmnheaders := $(wildcard $(CMNDIR)/*.hpp)

OBJS := game.o \
		minimap.o

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
	make dynamic -C $(CMNDIR)
endif


game.o: %.o: %.cpp 
	$(CC) $(CFLAGS) -c $<

minimap.o: %.o: %.cpp 
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(EXE) $(wildcard *.o)
