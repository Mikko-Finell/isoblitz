############## prelude

CC=g++
CFLAGS=-std=c++17 -Wfatal-errors
EXE=a.out
OBJ=brush.o \
    coordinate.o \
    editor.o \
    emitter.o \
    event.o \
    listener.o \
    map.o \
    position.o \
    shell.o \
    tile.o \
    ui.o
LIBS=-lsfml-system -lsfml-window -lsfml-graphics -lpthread

############## targets
# define in dependency order from left to right

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $(EXE) $(LIBS)

brush.o: brush.cpp
	$(CC) -c brush.cpp $(CFLAGS)
coordinate.o: coordinate.cpp
	$(CC) -c coordinate.cpp $(CFLAGS)
editor.o: editor.cpp
	$(CC) -c editor.cpp $(CFLAGS)
emitter.o: emitter.cpp
	$(CC) -c emitter.cpp $(CFLAGS)
event.o: event.cpp
	$(CC) -c event.cpp $(CFLAGS)
listener.o: listener.cpp
	$(CC) -c listener.cpp $(CFLAGS)
map.o: map.cpp
	$(CC) -c map.cpp $(CFLAGS)
position.o: position.cpp
	$(CC) -c position.cpp $(CFLAGS)
shell.o: shell.cpp
	$(CC) -c shell.cpp $(CFLAGS)
tile.o: tile.cpp
	$(CC) -c tile.cpp $(CFLAGS)
ui.o: ui.cpp
	$(CC) -c ui.cpp $(CFLAGS)
	
############## misc
# @ before rm to suppress messages

clean:
	@rm -rf $(EXE)  $(wildcard *.o) $(wildcard *.so)
