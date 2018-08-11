# editor Makefile

CC := clang++
CFLAGS := -std=c++17 -Wfatal-errors -I..
EXE := editor.out
LIBS := -lsfml-system -lsfml-window -lsfml-graphics -lpthread -ldl -lsqlite3 -lengine
CMNDIR := ../common

OBJS := brush.o \
	editor.o \
	map.o \
	shell.o \
	tile.o \
	ui.o

all: $(EXE)

$(EXE): $(OBJS) $(CMNDIR)/libengine.so
	$(CC) $(CFLAGS) -Wl,-rpath=$(CMNDIR) -L $(CMNDIR) $(LIBS) -o $@ $(OBJS)

brush.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

editor.o: %.o: %.cpp
	$(CC) $(CFLAGS) -c $<

map.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

shell.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

tile.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

ui.o: %.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<

$(CMNDIR)/libengine.so: $(wildcard $(CMNDIR)/*.cpp) $(wildcard $(CMNDIR)/*.hpp)
	make -C ../common

clean:
	rm -rf $(EXE) $(wildcard *.o)
