TARGETS=oddSum 2d-walk
CC=gcc
CFLAGS=-Wall -g

all : $(TARGETS)

$(TARGETS): %: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	@rm -f $(TARGETS) a.out *.o
