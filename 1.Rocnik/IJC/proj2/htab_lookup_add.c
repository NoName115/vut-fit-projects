/*2.DU na predmet IJC
* VUT - FIT
* Róbert Kolcún - xkolcu00
* Program pre ulohu B)
* 24.5.2016
*/

#include "htable.h"

struct htab_listitem *htab_lookup_add(htab_t *t, const char *key)
{
	if (t == NULL || key == NULL)
	{
		fprintf(stderr, "%s\n", "Error: invalid table or key");
		return NULL;
	}

	unsigned int hashIndex = t->hash_fun_ptr(key, t->htab_size);

	struct htab_listitem *item = t->ptr[hashIndex];
	struct htab_listitem *previousItem = NULL;

	while (item != NULL)
	{
		if (!strcmp(item->key, key))
		{
			return item;
		}

		previousItem = item;
		item = item->next;
	}

	//Prvok sa nenasiel, pridat na koniec zaznamu
	struct htab_listitem *newItem = malloc(sizeof(struct htab_listitem));
	if (newItem == NULL)
	{
		return NULL;
	}

	newItem->key = malloc((strlen(key) + 1) * sizeof(char));
	if (newItem->key == NULL)
	{
		free(newItem);
		return NULL;
	}

	//Inicializacia item-u a naplnenie hodnotamy
	(void) strcpy(newItem->key, key);
	newItem->data = 0;	//Riesi sa to v main-e
	newItem->next = NULL;

	//Navisenie poctu prvkov v tabulke
	t->n++;

	if (previousItem == NULL)	//Prazdny zoznam
	{
		t->ptr[hashIndex] = newItem;
	} else	//Prida na koniec zoznamu
	{
		previousItem->next = newItem;
	}


	return newItem;
}