/* Cvicenia IZP-CV_3, 2015/2016
 * Autor: Róbert Kolcún, FIT
 * <xkolcu00@stud.fit.vutbr.cz>
*/

#include <stdio.h>
#include <string.h>

// \xNNN, \b, \n, \t, \\, \?

int main(int argc, char *argv[])
{
  int i;

  while (i < argc) {
    if (!strcmp(argv[i], "--help")) 
	printf("P:%d - Pomocnik!!!!!\n", i);
    else printf("Arg %d:%s\n", i, argv[i]);
    i++;
  } 


  return 0;
}
