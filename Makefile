CC=gcc
CFLAGS=-Wall -D_XOPEN_SOURCE_EXTENDED
CLIBS=-lncursesw

all:
	@$(CC) $(CFLAGS) -o /bin/clock src/clock.c  $(CLIBS)
local:
	@$(CC) $(CFLAGS) -o clock src/clock.c $(CLIBS)
