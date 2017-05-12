CC=c99
CFLAGS=-Wall -Wextra -Wunreachable-code -ftrapv -D_POSIX_C_SOURCE=2

all: c-style

c-style: style.o
	$(CC) -o c-style $?

clean:
	rm -f *.o
