/* Cvicenia IZP-CV_3, 2015/2016
 * Autor: Róbert Kolcún, FIT
 * <xkolcu00@stud.fit.vutbr.cz>
*/

#include <stdio.h>

int main()
{
  char slovo[11];

  int code = scanf("%10s", slovo);
  while (code = 1)
  {
	printf("Nacitane slovo: %s\n", slovo);
	code = scanf("%10s", slovo);
  }

  return 0;
}
