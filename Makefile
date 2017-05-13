CC=c99
CFLAGS=-Wall -Wextra -Wunreachable-code -ftrapv -fPIC -g -D_POSIX_C_SOURCE=2

all: c-style

c_style_deps=style.o
c-style: $(c_style_deps)
	$(CC) -o $@ $(c_style_deps)

clean:
	rm -f *.o c-style
