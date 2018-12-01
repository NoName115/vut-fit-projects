/* Cvicenia IZP-CV_6, 2015/2016
 * Autor: Róbert Kolcún, FIT
 * <xkolcu00@stud.fit.vutbr.cz>
*/

#include <stdio.h>
#include <string.h>
#include <math.h>

double mocnina(int x, unsigned int count);
double faktorial(int x);

int main() {
  
  double s = 0;
  double t = 1.0;
  double eps = 0.1;
  int i = 0;

  int x = 5;

  while (fabs(t) > eps) {
  
    t = mocnina(x, i) / faktorial(i);
    //printf("%d %d\n", mocnina(x, i), faktorial(i));
    s += t;
    i++;
    //printf("%.2f\n", t);
  }

  printf("%g\n", s);
  
  //printf("%d %d\n", faktorial(3), mocnina(2,2));

  return 0;
}

double mocnina(int x, unsigned int count) {
  double vys = 1;
  int i;
  
  for (i = 0; i < count; i++) {
    vys *= x;
  }

  return vys;
}

double faktorial(int x) {
  int i;
  double vys = 1;

  for (i = x; i > 0; i--) {
  
    vys *= i;

  }

  return vys;
}
