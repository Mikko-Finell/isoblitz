# common Makefile

CC := clang++
CFLAGS := -std=c++17 -Wfatal-errors -g
LIBS := -lsfml-system -lsfml-window -lsfml-graphics -ldl

SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:cpp=o) 

all: $(OBJS)

$(OBJS): %.o: %.cpp
	$(CC) -c $< $(CFLAGS)

clean:
	@rm -rf $(OBJS)
