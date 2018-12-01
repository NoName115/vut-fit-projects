CFLAGS=-O2 -std=c99 -lm -Wall -Wextra -pedantic
LC_ALL=cs_CZ.utf8

all: primes primes-inline steg-decode

clean:
	rm -f *.o
	rm -f primes
	rm -f primes-inline
	rm -f steg-decode


#files .o
primes.o: primes.c error.h bit_array.h eratosthenes.h
	gcc $(CFLAGS) -c primes.c -o primes.o

error.o: error.c error.h
	gcc $(CFLAGS) -c error.c -o error.o

eratosthenes.o: eratosthenes.c eratosthenes.h bit_array.h error.h
	gcc $(CFLAGS) -c eratosthenes.c -o eratosthenes.o


ppm.o: ppm.c ppm.h error.h bit_array.h eratosthenes.h
	gcc $(CFLAGS) -c ppm.c -o ppm.o

steg-decode.o: steg-decode.c eratosthenes.h error.h bit_array.h ppm.h
	gcc $(CFLAGS) -c steg-decode.c -o steg-decode.o

#Final programs
primes: primes.o error.o eratosthenes.o
	gcc $(CFLAGS) primes.o error.o eratosthenes.o -o primes
#INLINE functions
primes-inline: primes.c error.o eratosthenes.c
	gcc $(CFLAGS) -DUSE_INLINE primes.c eratosthenes.c error.o -o primes-inline

#Finale decode program
steg-decode: steg-decode.o ppm.o eratosthenes.o error.o
	gcc $(CFLAGS) steg-decode.o ppm.o eratosthenes.o error.o -o steg-decode
