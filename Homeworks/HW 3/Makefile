TARGETS=diffusion monopoly
CC=gcc
CFLAGS=-Wall -g -std=c99

all : $(TARGETS)

$(TARGETS): %: %.c
	$(CC) $(CFLAGS) $< -o $@

clean: 
	@rm -f $(TARGETS) a.out *.o

sub:
	@zip $${PWD##*/}.zip $(TARGETS:%=%.c) Makefile
