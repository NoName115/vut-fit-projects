/* Cvicenia IZP-CV_2, 2015/2016
 * Autor: Róbert Kolcún, FIT
 * <xkolcu00@stud.fit.vutbr.cz>
*/

#include <stdio.h>

int main(int argc, char *argv[]) //** argv
{
  char *str1 = "Hello World";

  printf("Pocet argumentov je: %d \n", argc);
  
  int i;
 
  for (i = 0; i < argc; i++)
  {
    printf("Arg %d: %s\n", i, argv[i]);
  }

  printf("////////////////////////////// \n");
  
  printf("%02d: %s\n%02d: %s\n", 1, "Hello", 2, "World");
  printf("_%s_\n", str1);
  printf("_%15s_\n", str1);
  printf("_%-15.8s_\n", str1);

  return 0;
}
