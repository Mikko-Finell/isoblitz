# editor Makefile

CC := clang++
CFLAGS := -std=c++17 -Wfatal-errors -g -I..
EXE := editor.out
LIBS := -lsfml-system -lsfml-window -lsfml-graphics -lpthread -lsqlite3 
CMNDIR := ../common
LINKTYPE := dynamic
cmnheader := $(wildcard $(CMNDIR)/*.hpp)

OBJS := editor.o \
	editormenu.o \
	entityedit.o \
	entitybrush.o \
	entitymenu.o \
	entitycursor.o \
	tileedit.o \
	tilemenu.o \
	tilecursor.o \
	tilebrush.o

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

# base

editor.o: %.o: %.cpp $(CMNDIR)/util.hpp
	$(CC) $(CFLAGS) -c $<

editormenu.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

# entity 

entityedit.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

entitybrush.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

entitymenu.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

entitycursor.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

# tile

tileedit.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

tilemenu.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

tilecursor.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

tilebrush.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

# misc

clean:
	rm -rf $(EXE) $(wildcard *.o)
