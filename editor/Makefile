# editor Makefile

CC := clang++
CFLAGS := -std=c++17 -Wfatal-errors -I..
EXE := editor.out
LIBS := -lsfml-system -lsfml-window -lsfml-graphics -lpthread -ldl -lsqlite3
CMNDIR := ../common
SRCS := $(wildcard *.cpp)
CMNSRCS := $(wildcard $(CMNDIR)/*.cpp)
OBJS := $(SRCS:cpp=o) 
CMNOBJS := $(CMNSRCS:cpp=o)

all: $(EXE)

$(EXE): $(OBJS) $(CMNOBJS)
	$(CC) -o $@ $(OBJS) $(CMNOBJS) $(LIBS)

$(OBJS): %.o: %.cpp
	$(CC) -c $< $(CFLAGS)

$(CMNOBJS): %.o: %.cpp
	make -C $(CMNDIR)
	
clean:
	@rm -rf $(EXE) $(OBJS) $(CMNOBJS)
