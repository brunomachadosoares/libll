CC        = gcc
PROG_NAME = mem_analyze

CFLAGS  = -g -Wall -Werror -O2 \
            `pkg-config glib-2.0 --cflags`

LIBS    = `pkg-config glib-2.0 --libs`

BIN     = $(PROG_NAME)
OBJS    = analyze.o
HEADERS =


all: $(BIN)

$(BIN): $(HEADERS) $(OBJS) Makefile
	$(CC) $(OBJS) -o $@ $(CFLAGS) $(LIBS)

clean:
	rm -rf *.o $(BIN)

