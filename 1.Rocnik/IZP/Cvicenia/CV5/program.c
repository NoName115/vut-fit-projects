/* Cvicenia IZP-CV_5, 2015/2016
 * Autor: Róbert Kolcún, FIT
 * <xkolcu00@stud.fit.vutbr.cz>
*/

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  int i;
  int poz;

  for (i = 0; i < argc; i++) {
    if (strcmp(argv[i], "-h") == 0)
    {
      printf("Pozicia: %d\n", i+1);
      i = argc;
    }
    printf("a");
  }
}
