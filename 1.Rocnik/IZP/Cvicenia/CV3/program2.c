/* Cvicenia IZP-CV_3, 2015/2016
 * Autor: Róbert Kolcún, FIT
 * <xkolcu00@stud.fit.vutbr.cz>
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>


int main(int argc, char *argv[])
{
  bool hFlag = false;
  bool oFlag = false;
  bool cisloDef = false;
  bool chyba = false;
  int cislo = 0;
  int i = 1;
  
  while (i < argc) {
    if (!strcmp(argv[i], "-h") && !hFlag)
	hFlag = true;
    else if (!strcmp(argv[i], "-o") && !oFlag)
        oFlag = true;
    else if (!cisloDef) {
        cislo = atoi(argv[i]);
	cisloDef = true;
    } else 
	chyba = true;
    
    i++;
  }

  if (chyba)
    printf("Nastala chyba v argumentoch\n");
  else {
    if (hFlag) printf("Argument -h bol zadany\n");
    if (oFlag) printf("Argument -o bol zadany\n");
    if (cisloDef) printf("Bolo zadane cislo: %d\n", cislo);
  }

  printf("Fungujem!!!!\n");

  return 0;
}
