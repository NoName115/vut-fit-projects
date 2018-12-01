/* replaceText.c
 * Author: Róbert Kolcún, FIT 
 * <xkolcu00@stud.fit.vutbr.cz>
 * Priprava na skusku z IJC
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool str_subst(char *str, int max, unsigned p, unsigned len, const char *ins);

int main()
{
	char vstup[20] = "zabydazs";

	bool vysledok = str_subst(vstup, 20, 1, 3, "xaer");
}

bool str_subst(char *str, int max, unsigned p, unsigned len, const char *ins)
{
	bool changed = false;
	int pocet = 0;

	int rozdiel = strlen(str) - p - len;

	for (unsigned i = 0; i < p; ++i)
	{
		*str++;
		pocet++;
	}

	for (unsigned i = 0; i < strlen(ins); ++i)
	{
		*str++ = ins[i];
		pocet++;
	}

	fprintf(stderr, "%d\n", rozdiel);

	for (unsigned i = 0; i < len - strlen(ins) + rozdiel; ++i)
	{
		*str = ((char)*(str+(len - strlen(ins))));
		*str++;

		pocet++;
	}

	*str = '\0';

	fprintf(stdout, "%s\n", str-pocet);

	return changed;
}