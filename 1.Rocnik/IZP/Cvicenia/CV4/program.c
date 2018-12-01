/* Cvicenia IZP-CV_4, 2015/2016
 * Autor: Róbert Kolcún, FIT
 * <xkolcu00@stud.fit.vutbr.cz>
*/

#include <stdio.h>

int main()
{
  int a=0;
  int b=42;
  int *p;

  //p=&b;
  //*p=a;
  p = &a;
  (*p)++;

  printf("%d\n", p);

return 0;


  char s[20] = "Hello";
  int i;
  int poc = 0;

  for (i = 0; i < 20; i++) {
    if (s[i] == '\0') {
	printf("%d\n", poc);
	return 0;
    }
    poc++;
    printf("sdfsdf\n");
  }

  printf("Chyba", poc);

  return 0;
}
