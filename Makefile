SRC = $(wildcard src/*.c)

.PHONY: all clean

COMMON_FLAGS := -Wall -Wextra -g -lm
EXTRA_DEBUG_FLAGS := -fcolor-diagnostics -fansi-escape-codes
CC := cc

all: main

main: $(SRC)
	$(CC) ${COMMON_FLAGS} $^ -o $@

vscode-debug: $(SRC)
	$(CC) ${COMMON_FLAGS} ${EXTRA_DEBUG_FLAGS} $^ -o $@

lib: $(SRC)
	$(CC) ${COMMON_FLAGS} -fPIC -shared -o tbtc.so $^

clean:
	rm -rf *.o *~ main proxy.so
