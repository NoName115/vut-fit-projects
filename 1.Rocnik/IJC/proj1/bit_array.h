/* bit_array.h
 * Riesenie IJC-DU1, priklad a) 17.3.2016
 * Autor: Róbert Kolcún, FIT
 * Prelozene: gcc 4.9
 * Login: xkolcu00
 * Makra a inline fukcie pre pracu s bitovym polom
*/

#include "error.h"
#include <limits.h>

#ifndef BIT_ARRAY_H_INCLUDED
#define BIT_ARRAY_H_INCLUDED

typedef unsigned long bit_array_t[];

#define type_bits(array) (sizeof(array[0]) * CHAR_BIT)
#define ul_bits (sizeof(unsigned long) * CHAR_BIT)
#define ba_units(size) (size % ul_bits != 0 ? (size / ul_bits + 1) : (size / ul_bits))

#define ba_create(jmeno_pole, velikost) unsigned long jmeno_pole[ba_units(velikost) + 1] = {(unsigned long)(velikost)}


#define DU1_SET_BIT_(p, i, b) (b) != 0 ? (p[1 + i / type_bits(p)] |=  (1UL << i % type_bits(p))) \
				       : (p[1 + i / type_bits(p)] &= ~(1UL << i % type_bits(p)))

#define DU1_GET_BIT_(p, i) ((p[1 + i / type_bits(p)] >> i % type_bits(p)) & 1UL)


//If USE_LINE is not defined
#ifndef USE_INLINE

#define ba_size(jmeno_pole) jmeno_pole[0]

#define ba_set_bit(jmeno_pole, index, hodnota) (index >= jmeno_pole[0]) ? fatal_error("Index %ld mimo rozsah 0..%ld \n", (long)index, (long)(jmeno_pole[0] - 1)),0 : (DU1_SET_BIT_(jmeno_pole, index, hodnota))

#define ba_get_bit(jmeno_pole, index) (index >= jmeno_pole[0]) ? fatal_error("Index %ld mimo rozsah 0..%ld \n", (long)index, (long)(jmeno_pole[0] - 1)),0 : (DU1_GET_BIT_(jmeno_pole, index))

//If USE_LINE is defined
#else

inline int ba_get_bit(bit_array_t jmeno_pole, unsigned long index)
{
   if (index >= jmeno_pole[0])
   {
      fatal_error("Index %ld mimo rozsah 0..%ld \n", (long)index, (long)(jmeno_pole[0] - 1));
   }

   return DU1_GET_BIT_(jmeno_pole, index);
}

inline void ba_set_bit(bit_array_t jmeno_pole, unsigned long index, int hodnota)
{
   if (index >= jmeno_pole[0])
   {
      fatal_error("Index %ld mimo rozsah 0..%ld \n", (long)index, (long)(jmeno_pole[0] - 1));
   }

   DU1_SET_BIT_(jmeno_pole, index, hodnota);
}

#endif

#endif
