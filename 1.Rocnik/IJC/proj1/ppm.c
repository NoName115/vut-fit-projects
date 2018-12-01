/* ppm.c
 * Riesenie IJC-DU1, priklad b) 17.3.2016
 * Autor: Róbert Kolcún, FIT
 * Prelozene: gcc 4.9
 * Login: xkolcu00
 * ppm_read - funkcia pre nacitanie hodnot RGB farby pre kazdy pixel z obrazka
 * a ulozenia do pamate
 * ppm_write - vypisanie struktury ppm do suboru .ppm (obrazok)
*/

#include "ppm.h"

struct ppm * ppm_read(const char * filename)
{
   FILE *subor = fopen(filename, "r");
   if (subor == NULL)
   {
      warning_msg("ERROR: Opening file \n");
      return NULL;
   }

   unsigned long xSize,ySize;
   int range;
   int scanValue = fscanf(subor, "P6 %lu %lu %d%*c", &xSize, &ySize, &range);

   if (scanValue != 3 || range != 255)
   {
      if (fclose(subor) == EOF)
         warning_msg("ERROR: Closing file \n");

      warning_msg("ERROR: Loading data from file \n");
      return NULL;
   }

   struct ppm *ppmData = (struct ppm*) malloc(sizeof(struct ppm) + 3*xSize*ySize);
   if (ppmData == NULL)
   {
     if (fclose(subor) == EOF)
         warning_msg("ERROR: Closing file \n");

      warning_msg("ERROR: No memory \n");
      return NULL;
   }

   ppmData->xsize = xSize;
   ppmData->ysize = ySize;
   
   //printf("%lu, %lu \n", ppmData->xsize, ppmData->ysize);
   size_t count = fread(ppmData->data, 1, 3*xSize*ySize, subor);
   if (count < 3*xSize*ySize)
   {
      if (fclose(subor) == EOF)
         warning_msg("ERROR: Closing file \n");

      free(ppmData);
      warning_msg("ERROR: Loading data from file \n");
      return NULL;
   }
   
   if (fclose(subor) == EOF)
      warning_msg("ERROR: Closing file \n");

   return ppmData;
}

int ppm_write(struct ppm *p, const char * filename)
{
   FILE *subor = fopen(filename, "w");
   if (subor == NULL)
   {
      warning_msg("ERROR: Opening file \n");
      return -1;
   }

   (void) fprintf(subor, "P6\n%u %u\n255\n", p->xsize, p->ysize);
   
   size_t count = fwrite(p->data, 1, 3 * p->xsize * p->ysize, subor);
   if (count < (3 * p->xsize * p->ysize))
   {
      if (fclose(subor) == EOF)
         warning_msg("ERROR: Closing file \n");
      warning_msg("ERROR: Writing data to file \n");
      return -1;
   }

   if (fclose(subor) == EOF)
   {
      warning_msg("ERROR: Closing file \n");
      return -1;
   }

   return 0;
}
