
PRJ=c206
#
PROGS=$(PRJ)-test $(PRJ)-advanced-test $(PRJ)-advanced-test2
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm -g

all: $(PROGS)

$(PRJ)-test: $(PRJ).c $(PRJ)-test.c
	$(CC) $(CFLAGS) -o $@ $(PRJ).c $(PRJ)-test.c

$(PRJ)-advanced-test: $(PRJ).c $(PRJ)-advanced-test.c
	$(CC) $(CFLAGS) -o $@ $(PRJ).c $(PRJ)-advanced-test.c
	
$(PRJ)-advanced-test2: $(PRJ).c $(PRJ)-advanced-test2.c
	$(CC) $(CFLAGS) -o $@ $(PRJ).c $(PRJ)-advanced-test2.c

clean:
	rm -f *.o *.out $(PROGS)
