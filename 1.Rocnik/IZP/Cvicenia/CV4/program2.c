/* Cvicenia IZP-CV_4, 2015/2016
 * Autor: Róbert Kolcún, FIT
 * <xkolcu00@stud.fit.vutbr.cz>
*/

#include <stdio.h>

int main()
{
  printf("%d\n", oct2dec("777"));

  return 0;
}

int oct2dec(char *oct)
{
  int i;
  unsigned int val = 0;
  for (i = 0; oct[i] != '\0'; i++)
  {
    val += (oct[i] - '0')*Moc(i);
    printf("%d,%d\n", (oct[i] - '0'), Moc(i));
  }

  return val;
}

int Moc(int i) {
  int sum = 1;
  int j;
  for (j = 0; j < i; j++)
    sum *= 8;
  return sum;
}
