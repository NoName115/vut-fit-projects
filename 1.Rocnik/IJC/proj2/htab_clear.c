/*2.DU na predmet IJC
* VUT - FIT
* Róbert Kolcún - xkolcu00
* Program pre ulohu B)
* 24.5.2016
*/

#include "htable.h"

void htab_clear(htab_t *t)
{
	if (t == NULL || t->ptr == NULL)
	{
		return;
	}

	for (unsigned int i = 0; i < t->htab_size; i++)
	{
		if (t->ptr[i] == NULL)
			continue;

		struct htab_listitem *item = t->ptr[i];
		while (item != NULL)
		{
			struct htab_listitem *itemNext = item->next;

			if (item->key != NULL)
				free(item->key);

			if (item != NULL)
				free(item);

			item = itemNext;
		}
		t->ptr[i] = NULL;
	}

}