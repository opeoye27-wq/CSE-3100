CC=gcc
CFLAGS=-g -Wall -std=c99
TARGETS=dup-demo dup2-demo-input dup2-demo-output exec-dup file

all: $(TARGETS)

#Static Pattern Rules
$(TARGETS): % : %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf *.o *~ $(TARGETS)


