/* replace.c
 * Author: Róbert Kolcún, FIT 
 * <xkolcu00@stud.fit.vutbr.cz>
 * Priprava na skusku z IJC
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXIMUM 20

int replaceString(char *str, int max, const char *find, const char *chng);

int main()
{
   char vysledok[MAXIMUM] = "Tusdfie Tu sdmTuxci";
   const char najdi[] = "Tu";
   const char rep[] = "Ja";

   int pocet = replaceString(vysledok, MAXIMUM, najdi, rep);

   fprintf(stdout, "%s\n", vysledok);
   fprintf(stdout, "%d\n", pocet);

   return(0);
}

int replaceString(char *str, int max, const char *find, const char *chng)
{
	char *ret = NULL;

	int count = 0;

	while ((ret = strstr(str, find)) != NULL)
	{
		count++;

		int indx = ret - str;
		int lenChng = strlen(chng);

		for (int i = indx; i < indx + lenChng; ++i)
		{
			if (i >= max)
			{
				str[i] = '\0';
				return count;
			}

			str[i] = chng[i - indx];
		}
	}

	return count;
}