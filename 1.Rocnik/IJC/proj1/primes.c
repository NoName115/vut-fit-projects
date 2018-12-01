/* primes.c
 * Riesenie IJC-DU1, priklad a) 17.3.2016
 * Autor: Róbert Kolcún, FIT
 * Prelozene: gcc 4.9
 * Login: xkolcu00
 * Program pre vypisanie najvacsich 10 prvocisel pre limit cisla 202000000, zostupne
*/

#include <stdlib.h>
#include <stdio.h>

#include "eratosthenes.h"
#include "bit_array.h"
#include "error.h"

#define MAX_NUMBER 202000000

void WritePrimeNumbers()
{
   ba_create(eras, MAX_NUMBER);
   Eratosthenes(eras);

   int primesPrinted = 0;
   unsigned long i;
   for (i = MAX_NUMBER - 1; i > 0 && primesPrinted != 10; i--)
   {
      if (ba_get_bit(eras, i) == 0)
         primesPrinted++;
   }

   for (unsigned long j = i; j < MAX_NUMBER; j++)
   {
      if (ba_get_bit(eras, j) == 0)
         printf("%lu \n", j);
   }
}


int main()
{
   WritePrimeNumbers();

   return 0;
}
