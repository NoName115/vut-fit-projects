/* lineLength.c
 * Author: Róbert Kolcún, FIT 
 * <xkolcu00@stud.fit.vutbr.cz>
 * Priprava na skusku z IJC
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAXIMUM 50

short readLine(FILE *f, char *line, int max);

int main(int argc, char **argv)
{
	FILE *input;
	FILE *output;
	char vys[MAXIMUM] = "";
	short len;

	if (argc > 3 || argc == 1)
	{
		fprintf(stderr, "%s\n", "Invalid arguments");
		return -1;
	}

	input = fopen(argv[1], "rb");
	if (input == NULL)
	{
		fprintf(stderr, "%s\n", "Error: Opening file");
		return -1;
	}

	if (argc == 3)
	{
		output = fopen(argv[2], "wb");
		if (output == NULL)
		{
			fprintf(stderr, "%s\n", "Error: Opening output file");
			return -1;
		}
	}

	char vypis[] = "out.out";

	if (argc == 2)
	{
		output = fopen(vypis, "wb");
		if (output == NULL)
		{
			fprintf(stderr, "%s\n", "Error: Opening output file");
			return -1;
		}
	}

	while ((len = readLine(input, vys, MAXIMUM)) != EOF)
	{
		fprintf(output, "%d ", len);
		fprintf(output, "%s\n", vys);
	}

	fprintf(output, "%d ", len);
	fprintf(output, "%s\n", vys);
}

short readLine(FILE *f, char *line, int max)
{
	int c;
	int len = 1;

	while ((c = fgetc(f)) != EOF && c != '\n')
	{
		*line++ = c;
		*line = '\0';

		len++;

		if (len >= max)
		{
			while ((c = fgetc(f)) != '\n');
			fprintf(stdout, "%s\n", "Line cropped...");
			return len;
		}
	}

	if (len == 1)
	{
		*line = '\0';
	}

	if (c == EOF)
	{
		return EOF;
	}

	return len;
}