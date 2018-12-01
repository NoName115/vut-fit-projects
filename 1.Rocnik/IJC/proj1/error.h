/* error.h
 * Riesenie IJC-DU1, priklad b),a) 17.3.2016
 * Autor: Róbert Kolcún, FIT
 * Prelozene: gcc 4.9
 * Login: xkolcu00
 * Hlavickovy subor pre error.c
*/

#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

void warning_msg(const char *fmt, ...);

void fatal_error(const char *fmt, ...);

#endif
