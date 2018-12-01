/* steg-decode.c
 * Riesenie IJC-DU1, priklad b) 17.3.2016
 * Autor: Róbert Kolcún, FIT
 * Prelozene: gcc 4.9
 * Login: xkolcu00
 * Nacitanie RGB zloziek farieb, pixelov zo suboru .ppm, nasledne dekodovanie utajenej spravy
 * nachazajucej sa na LSB bitoch z bajtoch RGB zlozky.
*/

#include <ctype.h>

#include "bit_array.h"
#include "error.h"
#include "eratosthenes.h"
#include "ppm.h"

void PrintMessage(const char *filename)
{
   struct ppm *ppmData = ppm_read(filename);
   if (ppmData == NULL)
   {
      fatal_error("ERROR: Opening file \n");
   }

   unsigned long arraySize = 3 * ppmData->xsize * ppmData->ysize;

   //unsigned long * pole = (unsigned long *) malloc(sizeof(unsigned long) + arraySize / 8);
   unsigned long bit_size = arraySize;// / 8;
   unsigned long * pole = (unsigned long *) calloc(sizeof(unsigned long) + bit_size + 1, 1);
   if (pole == NULL)
   {
      free(ppmData);
      fatal_error("ERROR: No memory");
   }

   pole[0] = arraySize;
   Eratosthenes(pole);

   //printf("%d\n", ppmData->data[5]);

   char outChar[2] = {'\0'};
   unsigned int acCharBit = 0;
   
   for (unsigned long i = 2; i < arraySize; i++)
   {
      if (ba_get_bit(pole, i) == 0)
      {
         DU1_SET_BIT_(outChar, acCharBit, (ppmData->data[i] & 1));
         acCharBit++;

         if (acCharBit == 8 && outChar[1] == '\0')
         {
            printf("\n");
            break;
         } else
            if (acCharBit == 8)
            {
               if (!isprint(outChar[1]))
               {
                  free(ppmData);
                  free(pole);
                  fatal_error("ERROR: NonPrintable char \n");
                  break;
               }
               acCharBit = 0;
               printf("%c", outChar[1]);
            }
      }
      //Fatal_error pre neukoncenie '\0'
      if (i == arraySize - 1)
      {
         fatal_error("ERROR: String not ended by '\\0' \n");
      }
   }
   
   free(ppmData);
   free(pole);
}


int main(int argc, char *argv[])
{
   if (argc != 2)
   {
      warning_msg("Wrong number of arguments \n");
      return -1;
   }
   
   PrintMessage(argv[1]);

   return 0;
}
