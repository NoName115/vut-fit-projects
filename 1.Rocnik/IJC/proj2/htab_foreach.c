/*2.DU na predmet IJC
* VUT - FIT
* Róbert Kolcún - xkolcu00
* Program pre ulohu B)
* 24.5.2016
*/

#include "htable.h"

void htab_foreach(htab_t *t, void (*function)(const char *, unsigned int))
{
	if (t == NULL)
	{
		fprintf(stderr, "%s\n", "Error: Invalid table - foreach");
		return;
	}

	struct htab_listitem *item = NULL;

	for (unsigned int i = 0; i < t->htab_size; i++)
	{
		item = t->ptr[i];

		while (item != NULL)
		{
			function(item->key, item->data);
			item = item->next;
		}
	}
}
