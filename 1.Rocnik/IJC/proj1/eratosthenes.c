/* eratosthenes.c
 * Riesenie IJC-DU1, priklad a) 17.3.2016
 * Autor: Róbert Kolcún, FIT
 * Prelozene: gcc 4.9
 * Login: xkolcu00
 * Algoritmus - Eratosthenovo sito pre zistenie prvociselnych indexov v bitovom poli
*/

#include "eratosthenes.h"
#include "bit_array.h"
#include <math.h>

void Eratosthenes(bit_array_t pole)
{
   //poslednych 10 prvocisel
   unsigned long limit = (unsigned long) sqrt(pole[0]);
   unsigned long k = 2;

   ba_set_bit(pole, 0, 1);
   ba_set_bit(pole, 1, 1);

   for (unsigned long i = 2; i <= limit; i++)
   {
      if (ba_get_bit(pole, i) == 0)
      {
         k = 2;
         //printf("%lu \n", i);
         while (k*i < pole[0])
         {
            ba_set_bit(pole, k*i, 1);
            k++;
         }
      }
   }
}
