/* error.c
 * Riesenie IJC-DU1, priklad b),a) 17.3.2016
 * Autor: Róbert Kolcún, FIT
 * Prelozene: gcc 4.9
 * Login: xkolcu00
 * Funkci pre vypis varovneho hlasenia a fatalnej chyby s ukoncenim programu
*/

#include "error.h"

void warning_msg(const char *fmt, ...)
{
   va_list argList;
   va_start(argList, fmt);
   vfprintf(stderr, fmt, argList);
   va_end(argList);
}

void fatal_error(const char *fmt, ...)
{
   va_list argList;
   va_start(argList, fmt);
   vfprintf(stderr, fmt, argList);
   va_end(argList);
   
   exit(1);
}
