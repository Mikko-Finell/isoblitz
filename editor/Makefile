# editor Makefile

CC := clang++
CFLAGS := -std=c++17 -Wfatal-errors -I..
EXE := editor.out
LIBS := -lsfml-system -lsfml-window -lsfml-graphics -lpthread -lsqlite3 -lrepl
CMNDIR := ../common
LINKTYPE := dynamic
cmnheader := $(wildcard $(CMNDIR)/*.hpp)

OBJS := editor.o \
	map.o \
	shell.o \
	tilefactory.o \
	tilemenu.o

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

editor.o: %.o: %.cpp $(CMNDIR)/util.hpp
	$(CC) $(CFLAGS) -c $<

map.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

shell.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

tilefactory.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

tilemenu.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(EXE) $(wildcard *.o)
