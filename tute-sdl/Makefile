# $Id: Makefile 4 2006-07-03 12:40:56Z aholkner $

DEBUG = -g

OPTIMISE = 

CFLAGS = `sdl-config --cflags` $(DEBUG) $(OPTIMISE) -Werror -Wall
LDFLAGS = `sdl-config --libs` -lGL -lGLU -lglut

HEADERS = sdl-base.h
OBJECTS = tute-sdl.o sdl-base.o
EXE = example

$(EXE): $(OBJECTS) $(HEADERS)
	gcc -o $@ $(OBJECTS) $(LDFLAGS)

clean:
	rm -f $(EXE) $(OBJECTS)
