CFLAGS= -std=c++11 -O2 -Wall -Wreturn-type -Wunused-variable -g
PFLAGS=/usr/local/lib/libsimlib.so -lm
SOURCES=main.cpp
SIMLIB_DIR_SO = /usr/local/lib/libsimlib.so



CC=g++
all: ims
ims: main.cpp $(SIMLIB_DIR_SO)
	$(CC) $(CFLAGS) -o $@ main.cpp $(PFLAGS)
