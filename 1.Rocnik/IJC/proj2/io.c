/*2.DU na predmet IJC
* VUT - FIT
* Róbert Kolcún - xkolcu00
* Program pre ulohu B)
* 24.5.2016
*/

#include "io.h"

int get_word(char *s, int max, FILE *f)
{
	if (f == NULL)
		return EOF;

	int testChar;
	int actualChar = 1;

	while ((testChar = fgetc(f)) != EOF && isspace(testChar));

	if (testChar == EOF)
		return EOF;

	*s = testChar;	//na poziciu 0 zapiseme znak

	while (!isspace(testChar = fgetc(f)))
	{
		if (testChar == EOF)
		{
			s[actualChar] = '\0';
			return EOF;
		}

		s[actualChar] = testChar;
		actualChar++;

		if (actualChar + 2 > max)
		{
			s[actualChar] = '\0';
			while (!isspace(fgetc(f))) actualChar++;	//Zahodit zbytok slova
			return actualChar;
			//fprintf(stderr, "%s\n", "Error: Word limit reached");
		}
	}

	s[actualChar] = '\0';

	return actualChar;
}


//-------------------DEBUGGING-----------------------
/*
int main(int argc, char **argv)
{
	FILE *f = fopen(argv[1], "r");
	if (f == NULL)
	{
		fprintf(stderr, "%s\n", "Error: File openning");
		return 1;
	}

	char *s = malloc(100*sizeof(char));
	int d = get_word(s, 2, f);

	printf("%s %d\n", s, d);

	s = malloc(100*sizeof(char));
	d = get_word(s, 2, f);

	printf("%s %d\n", s, d);

	return 0;
}
*/