CC=gcc
CFLAGS=-Wall -D_XOPEN_SOURCE_EXTENDED
CLIBS=-lncursesw

all:
	@$(CC) $(CFLAGS) -o clock clock.c $(CFLAGS) $(CLIBS)
