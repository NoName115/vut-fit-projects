/*2.DU na predmet IJC
* VUT - FIT
* Róbert Kolcún - xkolcu00
* Program pre ulohu B)
* 24.5.2016
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htable.h"
#include "io.h"

#define WORDLIMIT 127
const unsigned int tabSize = 49157;

void printTable(const char *key, unsigned int value);

int main(void)
{
	htab_t *table = htab_init(tabSize);
	if (table == NULL)
	{
		fprintf(stderr, "%s\n", "Error: Alocation table");
		return -1;
	}

	bool errorPrinted = false;
	int wordLength = 0;
	char word[WORDLIMIT + 1];
	while ((wordLength = get_word(word, WORDLIMIT, stdin)) != EOF)
	{
		struct htab_listitem *item = NULL;

		if (!errorPrinted && wordLength >= WORDLIMIT)
		{
			fprintf(stderr, "%s\n", "Error: Word limit reached");
			errorPrinted = true;
		}

		if ((item = htab_lookup_add(table, word)) == NULL)	//Prida slovo do tabulky
		{
			fprintf(stderr, "%s\n", "Error: Alocation item in table");
			htab_free(table);
			return -1;
		}

		item->data++;
	}

	htab_foreach(table, printTable);

	htab_free(table);

	return 0;
}

void printTable(const char *key, unsigned int value)
{
    printf("%s\t%u\n", key, value);
}