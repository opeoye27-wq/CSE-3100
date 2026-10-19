CC := gcc
CFLAGS := -g -Wall -std=c99 -fsanitize=address
TARGETS := linkedlist-main

all: $(TARGETS)

linkedlist-main: linkedlist-main.c linkedlist.c 
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf *.o $(TARGETS)
