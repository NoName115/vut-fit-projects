/*2.DU na predmet IJC
* VUT - FIT
* Róbert Kolcún - xkolcu00
* Program pre ulohu B)
* 24.5.2016
*/

#include "htable.h"

htab_t *htab_init(unsigned int size)
{
	htab_t *newTab = malloc(sizeof(htab_t) + size * sizeof(struct htab_listitem *));

	if (newTab == NULL)
	{
		return NULL;
	}


	for (unsigned int i = 0; i < size; i++)
	{
		newTab->ptr[i] = NULL;
	}

	newTab->htab_size = size;
	newTab->n = 0;
	newTab->hash_fun_ptr = hash_function;

	return newTab;
}