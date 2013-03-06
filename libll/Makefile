CC        = gcc
PROG_NAME = libll.a

CFLAGS  = -Wall -Werror -rdynamic \
			`pkg-config glib-2.0 --cflags`

LIBS    = `pkg-config glib-2.0 --libs`

BIN     = $(PROG_NAME)
SRC		= libll.c
OBJS    = libll.o
HEADERS = libll.h

all:
	$(CC) -c $(SRC) -o $(OBJS) $(CFLAGS) $(LIBS)

clean:
	rm -rf *.o $(PROG_NAME)

lib:
	ar cru $(PROG_NAME) *.o
