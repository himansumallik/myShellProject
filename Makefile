CC = gcc
CFLAGS = -Wall -Wextra -I.
LDFLAGS = -lreadline

all: shellprogram

shellprogram: shell.o library.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

shell.o: shell.c library.h
	$(CC) $(CFLAGS) -c shell.c

library.o: library.c library.h
	$(CC) $(CFLAGS) -c library.c

clean:
	rm -f shellprogram *.o

