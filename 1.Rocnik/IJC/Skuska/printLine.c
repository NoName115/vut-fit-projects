/* printLine.c
 * Author: Róbert Kolcún, FIT 
 * <xkolcu00@stud.fit.vutbr.cz>
 * Priprava na skusku z IJC
*/

#include <stdio.h>
#include <stdlib.h>

#define MAXIMUM 64

int read_line(char *str, int n, FILE *f);

int main(int argc, char **argv)
{
	int len;
	int max;
	FILE *input;

	if (argc > 3)
	{
		fprintf(stderr, "%s\n", "Invalid arguments");
		return -1;
	}

	if (argc == 3)
	{
		input = fopen(argv[2], "rb");
		if (input == NULL)
		{
			fprintf(stderr, "%s\n", "Error: Opening file");
			return -1;
		}
	}

	if (argc == 2)
	{
		input = stdin;
	}

	max = atoi(argv[1]);
	char vys[MAXIMUM];

	int por = 1;

	while ((len = read_line(vys, MAXIMUM, input)) != EOF)
	{
		if (len > max)
		{
			fprintf(stdout, "%d %d %s\n", por, len, vys);
		}

		por++;
	}
}

int read_line(char *str, int n, FILE *f)
{
	int c;
	int len = 0;
	while ((c = fgetc(f)) != EOF && c != '\n')
	{
		*str++ = c;
		*str = '\0';

		len++;

		if (len >= n - 1)
		{
			while ((c = fgetc(f)) != '\n');
			return len + 1;
		}
	}

	if (c == '\n')
	{
		return len;
	}

	if (c == EOF)
	{
		return EOF;
	}

	return len;
}