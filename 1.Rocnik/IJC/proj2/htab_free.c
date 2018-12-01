/*2.DU na predmet IJC
* VUT - FIT
* Róbert Kolcún - xkolcu00
* Program pre ulohu B)
* 24.5.2016
*/

#include "htable.h"

void htab_free(htab_t *t)
{
	if (t == NULL)
	{
		return;
	}

	htab_clear(t);
	free(t);
	t = NULL;
}