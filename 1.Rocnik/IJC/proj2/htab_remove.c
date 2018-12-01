/*2.DU na predmet IJC
* VUT - FIT
* Róbert Kolcún - xkolcu00
* Program pre ulohu B)
* 24.5.2016
*/

#include "htable.h"

void htab_remove(htab_t *t, const char *key)
{
	if (t == NULL || key == NULL)
	{
		fprintf(stderr, "%s\n", "Error: invalid table or key");
		return;
	}

	bool itemFound = false;
	unsigned int hashIndex = t->hash_fun_ptr(key, t->htab_size);

	struct htab_listitem *item = t->ptr[hashIndex];
	struct htab_listitem *previousItem = NULL;

	while (item != NULL)
	{
		if (!strcmp(item->key, key))	//Nasiel pozadovanu polozku
		{
			itemFound = true;

			if (previousItem == NULL && item->next == NULL)	//Prvy v zozname a jediny
			{
				t->ptr[hashIndex] = NULL;
			}
			else if (previousItem != NULL && item->next == NULL) //Posledny v zoznam
			{
				previousItem->next = NULL;
			}
			else if (previousItem != NULL && item->next != NULL) //Ma oboch susedov
			{
				previousItem->next = item->next;

			} else {  //Prvy v zozname a ma suseda
				t->ptr[hashIndex] =  item->next;
			}

			free(item->key);
			item->next = NULL;
			free(item);
			t->n--;

			break;
		}

		//Nakoniec
		previousItem = item;
		item = item->next;
	}

	if (!itemFound)
	{
		fprintf(stderr, "%s\n", "Error: Invalid key, item not found");
		return;
	}
}
