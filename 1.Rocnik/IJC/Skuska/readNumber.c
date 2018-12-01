/* readNumber.c
 * Author: Róbert Kolcún, FIT 
 * <xkolcu00@stud.fit.vutbr.cz>
 * Priprava na skusku z IJC
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX 20

int numberFromFile(FILE *f, char *vys, int max);

int main(int argc, char **argv)
{
	FILE *inputFile;

	if (argc > 2)
	{
		fprintf(stderr, "%s\n", "Invalid arguments");
		return -1;
	}

	if (argc == 1)
	{
		inputFile = stdin;
	}

	if (argc == 2)
	{
		inputFile = fopen(argv[1], "rb");
		if (inputFile == NULL)
		{
			fprintf(stderr, "%s\n", "Error: Opening file");
			return -1;
		}
	}

	//*********NACITAVANIE CISIEL*************
	int len = 0;
	char vys[MAX] = "";

	while ((len = numberFromFile(inputFile, vys, MAX)) != EOF)
	{
		fprintf(stdout, "%s\n", vys);
	}

	return 0;
}

int numberFromFile(FILE *f, char *vys, int max)
{
	int c;
	int len = 1;

	//Ignorovanie prvych NonDigit znakov
	while ((c = fgetc(f)) && c != EOF && !isdigit(c));

	if (c == EOF)
	{
		vys = NULL;
		return EOF;
	}

	*vys++ = c;
	*vys = '\0';

	while ((c = fgetc(f)) != EOF && isdigit(c))
	{
		*vys++ = c;
		*vys = '\0';

		len++;

		if (len >= max)
		{
			return len;
		}
	}

	return len;
}