CFLAGS=-std=c99 -Wall -Wextra  -Werror -pedantic

all: clean number string length length2 printer replacer

clean:
	rm -f spus
	rm -f number
	rm -f string
	rm -f length
	rm -f length2
	rm -f printer
	rm -f replacer


#Programy
number:
	gcc $(CFLAGS) readNumber.c -o number
string:
	gcc $(CFLAGS) replace.c -o string
length:
	gcc $(CFLAGS) wordLength.c -o length
length2:
	gcc $(CFLAGS) lineLength.c -o length2
printer:
	gcc $(CFLAGS) printLine.c -o printer
replacer:
	gcc $(CFLAGS) replaceText.c -o replacer