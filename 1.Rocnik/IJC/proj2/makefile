#############################
# 2.DU na predmet IJC		#
# VUT - FIT					#
# Róbert Kolcún - xkolcu00	#
# Makefile pre ulohu A),B)	#
# 24.5.2016					#
#############################

CFLAGS=-std=c99 -pedantic -Wall -Wextra
CPPFLAGS=-std=c++11 -pedantic -Wall

LIBFILES=hash_function.o htab_clear.o htab_init.o htab_init2.o htab_lookup_add.o htab_free.o htab_foreach.o htab_remove.o

all: tail tail2 wordcount wordcount-dynamic

tail: tail.c
	gcc $(CFLAGS) -o tail tail.c

tail2: tail2.cc
	g++ $(CPPFLAGS) -o tail2 tail2.cc

#staticka verze
wordcount: wordcount.o io.o libhtable.a
	gcc $(CFLAGS) -o wordcount -static io.o wordcount.o libhtable.a

#dynamicka verze
wordcount-dynamic: wordcount.o io.o libhtable.so
	gcc $(CFLAGS) -o wordcount-dynamic wordcount.o io.o libhtable.so

#----------------------------KNIZNICE---------------------------
#staticka knihovna
libhtable.a: $(LIBFILES)
	ar crs libhtable.a $(LIBFILES)

#sdilena (dynamicka) knihovna
libhtable.so: $(LIBFILES)
	gcc $(CFLAGS) -shared -fPIC -o libhtable.so $(LIBFILES)

#---------------------------.o FILES----------------------------

wordcount.o: wordcount.c htable.h
	gcc $(CFLAGS) -c wordcount.c -o wordcount.o

io.o:
	gcc $(CFLAGS) -c io.c -o io.o

hash_function.o: hash_function.c hash_function.h
	gcc $(CFLAGS) -fPIC -c hash_function.c -o hash_function.o

htab_clear.o: htab_clear.c htable.h
	gcc $(CFLAGS) -fPIC -c htab_clear.c -o htab_clear.o

htab_init.o: htab_init.c htable.h
	gcc $(CFLAGS) -fPIC -c htab_init.c -o htab_init.o

htab_init2.o: htab_init2.c htable.h
	gcc $(CFLAGS) -fPIC -c htab_init2.c -o htab_init2.o

htab_lookup_add.o: htab_lookup_add.c htable.h
	gcc $(CFLAGS) -fPIC -c htab_lookup_add.c -o htab_lookup_add.o

htab_free.o: htab_free.c htable.h
	gcc $(CFLAGS) -fPIC -c htab_free.c -o htab_free.o

htab_foreach.o: htab_foreach.c htable.h
	gcc $(CFLAGS) -fPIC -c htab_foreach.c -o htab_foreach.o

htab_remove.o: htab_remove.c htable.h
	gcc $(CFLAGS) -fPIC -c htab_remove.c -o htab_remove.o

clean:
	rm -f *.o tail tail2 libhtable.a libhtable.so wordcount wordcount-dynamic 
