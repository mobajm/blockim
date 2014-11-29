.PHONY: clean, mrproper

SOURCES=main.c graphic.c board.c square.c shape.c tablet.c player.c
OBJECTS=$(SOURCES:.c=.o)

CC=gcc
CFLAGS=-W -Wall -Wextra -Wuninitialized -Wwrite-strings -Wno-missing-braces -ansi -pedantic
CLIBS=`sdl-config --cflags`

LDFLAGS=
LIBS=-lm `sdl-config --libs` -lSDL_image

EXEC=blockim

all: CFLAGS += -O3
debug: CFLAGS += -g -DDEBUG

# all
all : $(EXEC)
debug : $(EXEC)

$(EXEC) : $(OBJECTS)
	$(CC) $^ -o $(EXEC) $(LDFLAGS) $(LIBS)

%.o : %.c 
	$(CC) -c $< -o $@ $(CFLAGS) $(CLIBS)

clean: 
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
