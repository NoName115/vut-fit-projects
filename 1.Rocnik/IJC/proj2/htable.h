/*2.DU na predmet IJC
* VUT - FIT
* Róbert Kolcún - xkolcu00
* Program pre ulohu B)
* 24.5.2016
*/

#ifndef HTABLE_H_INCLUDED
#define HTABLE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hash_function.h"

struct htab_listitem
{
	char *key;					//Ukazatel na retazec
	long int data;				//Pocet vyskytov a
	struct htab_listitem *next;	//Ukazatel na dalsi prvok
};

typedef struct htab_t
{
	unsigned int htab_size;		//velkost pola
		//ukazovatel na hash-ovaciu funkciu
	unsigned int (*hash_fun_ptr)(const char *, unsigned);
	unsigned int n;					//aktualny pocet zaznamov
	struct htab_listitem *ptr[];	//Pole ukazetelov na ITEM
} htab_t;


//-------------------FUNKCIE----------------------------------
htab_t *htab_init(unsigned int size);
htab_t *htab_init2(unsigned int size, unsigned int (*hashfunction)(const char *str, unsigned htab_size));

struct htab_listitem *htab_lookup_add(htab_t *t, const char *key);

void htab_foreach(htab_t *t, void (*function)(const char *, unsigned int));

void htab_remove(htab_t *t, const char *key);

void htab_clear(htab_t *t);

void htab_free(htab_t *t);	//Vola funkciu clear


#endif