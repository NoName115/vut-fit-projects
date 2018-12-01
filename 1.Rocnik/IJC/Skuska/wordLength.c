/* wordLength.c
 * Author: Róbert Kolcún, FIT 
 * <xkolcu00@stud.fit.vutbr.cz>
 * Priprava na skusku z IJC
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define LENGTH 10
#define MAXIMUM 50

int getMyLine(FILE *f, int max, char *str);

int main(int argc, char **argv)
{
	FILE *input;
	char riadok[MAXIMUM] = "";
	int len;

	if (argc > 2)
	{
		fprintf(stderr, "%s\n", "Invalid arguments");
		return -1;
	}

	if (argc == 1)
	{
		input = stdin;
	}

	if (argc == 2)
	{
		input = fopen(argv[1], "rb");
		if (input == NULL)
		{
			fprintf(stderr, "%s\n", "Error: Opening file");
			return -1;
		}
	}


	while ((len = getMyLine(input, MAXIMUM, riadok)) != EOF)
	{
		if (len == LENGTH - 1 || len == LENGTH + 1)
		{
			fprintf(stdout, "%s\n", riadok);
		}
	}

	return 0;
}

int getMyLine(FILE *f, int max, char *str)
{
	int c;
	int len = 1;

	//Preskocit zaciatocne nonAlphaNumeric
	while ((c = fgetc(f)) != EOF && !isalnum(c));

	if (c == EOF)
	{
		str = NULL;
		return EOF;
	}

	*str++ = c;
	*str = '\0';

	while ((c = fgetc(f)) != EOF && isalnum(c))
	{
		*str++ = c;
		*str = '\0';

		len++;
		if (len >= max)
		{
			fprintf(stderr, "%s\n", "Cropping line...");
			return len;
		}
	}

	return len;
}

