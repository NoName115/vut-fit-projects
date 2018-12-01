/* ppm.h
 * Riesenie IJC-DU1, priklad b) 17.3.2016
 * Autor: Róbert Kolcún, FIT
 * Prelozene: gcc 4.9
 * Login: xkolcu00
 * Hlavickovy subor pre ppm.c
*/

#ifndef PPM_H_INCLUDED
#define PPM_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "bit_array.h"
#include "eratosthenes.h"

struct ppm {
  unsigned xsize;
  unsigned ysize;
  char data[];
};

struct ppm * ppm_read(const char * filename);
int ppm_write(struct ppm *p, const char * filename);

#endif
