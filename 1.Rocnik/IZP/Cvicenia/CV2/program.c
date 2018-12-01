/* Cvicenia IZP-CV_2, 2015/2016
 * Autor: Róbert Kolcún, FIT
 * <xkolcu00@stud.fit.vutbr.cz>
*/

#include <stdio.h>

int main()
{
  char *str1 = "Prve cislo:";
  char *str2 = "Druhe cislo:";

  int i = 42 + 5;
  float j = 42.56;
  int k = 32;
 
  printf("Mame cisla, i: %3d, j: %3.3f, k: %03d, \nk: %x, %o\n", i, j, k, 255, 255);

  if (i > 42)
    printf("%s %d je vacsie ako %s %0.2f \n", str1, i, str2,j);
  else 
    printf("Cislo: %0.2f je vacsie ako: %d \n", j, i);

  return 0;
}
