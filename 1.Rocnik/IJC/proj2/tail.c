/*2.DU na predmet IJC
* VUT - FIT
* Róbert Kolcún - xkolcu00
* Program pre ulohu A)
* 24.5.2016
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define defaultNumberOfLines 10
#define buffSize 510

int IsDigit(char *v)
{
	int i = 0;
	while (v[i] != '\0') i++;

	for (int j = 0; j < i; j++)
	{
		if (v[j] > 57 || v[j] < 48)
			return 0;
        }

	return 1;
}

void tail(FILE *file, int numberOfLines)
{
	if (numberOfLines == 0)
		return;
	if (file == NULL)
		file = stdin;

	char stringBuffer[numberOfLines][buffSize + 2];
	int testChar;
	int i = 0;
	int looped = 0;
	int printedError = 0; 

	while (fgets(stringBuffer[i], buffSize + 2, file) != NULL)
	{
		if (strchr(stringBuffer[i], '\n') == NULL)
		{
			if (!printedError)
			{
				fprintf(stderr, "%s\n", "Line cropped");
				printedError++;
			}
			stringBuffer[i][buffSize] = '\n';
			while ((testChar = fgetc(file)) != '\n' && testChar != EOF);
		}
		
		i++;
		if (i > numberOfLines - 1)
		{
			i = 0;
			looped = 1;
		}
	}

	//Vypis
	if (looped)
	{
		for (int j = i; j < numberOfLines; j++)
		{
			printf("%s", stringBuffer[j]);
		}
	}

	for (int k = 0; i > k; k++)
	{
		printf("%s", stringBuffer[k]);
	}


}

int main(int argc, char **argv)
{
	FILE *file;
	int numberOfLines = defaultNumberOfLines;

	if (argc > 4)
	{
		fprintf(stderr, "%s\n", "Error: Too many arguments");
		return 1;
	}

	if (argc == 1)
	{
		file = NULL;
	}

	if (argc == 2)
	{
		file = fopen(argv[1], "rb");
		
		if (file == NULL)
		{
			fprintf(stderr, "%s\n", "Error: Opening file");
			return 1;
		}
	}

	if (argc == 3)
	{
		if (!strcmp(argv[1], "-n"))
		{
			if (IsDigit(argv[2]))
			{
				numberOfLines = atoi(argv[2]);
				file = NULL;
			} else
			{
				fprintf(stderr, "%s %s\n", "Error: Wrong argument", argv[2]);
				return 1;
			}
		} else
		{
			fprintf(stderr, "%s %s\n", "Error: Wrong argument", argv[1]);
			return 1;
		}
	}

	if (argc == 4)
	{
		if (!strcmp(argv[1], "-n"))
		{
			if (IsDigit(argv[2]))
			{
				numberOfLines = atoi(argv[2]);
				file = fopen(argv[3], "rb");

				if (file == NULL)
				{
					fprintf(stderr, "%s\n", "Error: Opening file");
					return 1;
				}
			} else
			{
				fprintf(stderr, "%s %s\n", "Error: Wrong argument", argv[2]);
				return 1;
			}
		} else
		{
			fprintf(stderr, "%s %s\n", "Error: Wrong argument", argv[1]);
			return 1;
		}
	}

	tail(file, numberOfLines);

	return 0;
}
