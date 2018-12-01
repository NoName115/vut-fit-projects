/*2.DU na predmet IJC
* VUT - FIT
* Róbert Kolcún - xkolcu00
* Program pre ulohu B)
* 24.5.2016
*/

#include "hash_function.h"

unsigned int hash_function(const char *str, unsigned htab_size)
{
	unsigned int h = 0;
	const unsigned char *p;
	
	for (p = (const unsigned char*)str; *p!='\0'; p++)
		h = 65599*h + *p;
	
	return h % htab_size;
}