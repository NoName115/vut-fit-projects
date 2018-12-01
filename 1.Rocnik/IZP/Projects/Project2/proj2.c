/* Projekt 2 IZP, 2015/2016
 * Autor: Róbert Kolcún, FIT
 * <xkolcu00@stud.fit.vutbr.cz>
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define ERROR printf("ERROR: Invalid Input\n"); return 0;

int countTaylorBinarySearch(double x, double eps);
int countCfBinarySearch(double x, double eps);

int countTaylorIterations(double x, double eps);
int countCfIterations(double x, double eps);

double taylor_log(double x, unsigned int n);
double cfrac_log(double x, unsigned int n);

//Z hodnot fir a sec, najde maximum ktore funkcia vrati
int findMaximum(int fir, int sec) {
    if (fir > sec) return fir;
    else return sec;

    return -1;
}

//Funckia vrati absolutnu hodnotu vstupneho argumentu X
double absolute(double x) {
    return x < 0 ? -x : x;
}

int main(int argc, char *argv[])
{
    double x;
    unsigned int n;

    double min;
    double max;
    double eps;

    //Spustenie programu pre vypicet logaritmu
    if (argc == 4 && !strcmp(argv[1], "--log")) {

        if (sscanf(argv[2], "%lg", &x) != 1 || (argv[3])[0] == '-' || sscanf(argv[3], "%u", &n) != 1) {
            ERROR;
        }
	//if (n == 0) {
        //    ERROR;
        //}

        printf("log(%g) = %.12g\n", x, log(x));

        //Vypis CF logaritmu
        double cfVys = cfrac_log(x, n);
        printf("cf_log(%g) = %.12g\n", x, cfVys);

        //Vypis Taylor logaritmu
        double taylorVys = taylor_log(x, n);
        printf("taylor_log(%g) = %.12g\n", x, taylorVys);

        return 0;
    } else
    //Vypocet poctu iteracii
    if (argc == 5 && !strcmp(argv[1], "--iter")) {

        if (sscanf(argv[2], "%lg", &min) != 1 || sscanf(argv[3], "%lg", &max) != 1 || sscanf(argv[4], "%lg", &eps) != 1) {
            ERROR;
	}
        if (eps <= 0) {
            ERROR;
        }

        //Vypis MIN a MAX pre log z math.h
        printf("log(%g) = %.12g\n", min, log(min));
        printf("log(%g) = %.12g\n", max, log(max));

        //--------------------BINARY SEARCH-----------------------
        //Vypis a vypocet poctu iteracii pre MAX a MIN - CF
        int cfIterationsMax = findMaximum(countCfBinarySearch(min, eps), countCfBinarySearch(max, eps));
        printf("continued fraction iterations = %d\n", cfIterationsMax);

        double cfVys = cfrac_log(min, cfIterationsMax);
        printf("cf_log(%g) = %.12g\n", min, cfVys);

        cfVys = cfrac_log(max, cfIterationsMax);
        printf("cf_log(%g) = %.12g\n", max, cfVys);

        //Vypis a vypocet poctu iteracii pre MAX a MIN - Taylor
        int taylorIterationsMax = findMaximum(countTaylorBinarySearch(min, eps), countTaylorBinarySearch(max, eps));
        printf("continued fraction iterations = %d\n", taylorIterationsMax);

        double taylorVys = taylor_log(min, taylorIterationsMax);
        printf("taylor_log(%g) = %.12g\n", min, taylorVys);

        taylorVys = taylor_log(max, taylorIterationsMax);
        printf("taylor_log(%g) = %.12g\n", max, taylorVys);
    
        return 0;
    } else {
        ERROR;
    }
}

//Funckia pre vypocet poctu iteracii pomocou binarneho vyhladavania - Taylor
int countTaylorBinarySearch(double x, double eps) {
    unsigned int zac = 1;
    unsigned int kon = 1000;
    unsigned int vyh;

    if (x <= 0) return 0;

    while (absolute(taylor_log(x, kon) - log(x)) > eps) {
        zac = kon;
        kon += 1000;
    }

    vyh = (zac + kon) / 2;

    while (1) {
        if (absolute(taylor_log(x, vyh) - log(x)) < eps) {
            kon = vyh;
            vyh = (zac + kon) / 2;
            if (vyh == kon) {
                return kon;
            }
        } else {
            zac = vyh;
            vyh = (zac + kon) / 2;
            if (vyh == zac) {
                return kon;
            }
        }
    }
}

//Funkcia pre vypocet poctu iteracii pomocou binarneho vyhladavania - CF
int countCfBinarySearch(double x, double eps) {
    unsigned int zac = 1;
    unsigned int kon = 1000;
    unsigned int vyh;

    if (x <= 0) return 0;

    while (absolute(cfrac_log(x, kon) - log(x)) > eps) {
        zac = kon;
        kon += 1000;
    }

    vyh = (zac + kon) / 2;

    while (1) {
        if (absolute(cfrac_log(x, vyh) - log(x)) < eps) {
            kon = vyh;
            vyh = (zac + kon) / 2;
            if (vyh == kon) {
                return kon;
            }
        } else {
            zac = vyh;
            vyh = (zac + kon) / 2;
            if (vyh == zac) {
                return kon;
            }
        }
    }
}

//Vypocet iteracii pomocou rozielu daneho clena a log z math.h
int countTaylorIterations(double x, double eps) {
    unsigned int i = 1;

    while (absolute(taylor_log(x, i) - log(x)) > eps) {
        i++;
    }

    return i;
}

//Vypocet interacii pomocou rozdielu daneho clena a log z math.h
int countCfIterations(double x, double eps) {
    unsigned int i = 1;

    while (absolute(cfrac_log(x, i) - log(x)) > eps) {
        i++;
    }

    return i;
}

//Vypocet Taylor logaritmu pre vstupnu hodnotu X a pocet interacii N
double taylor_log(double x, unsigned int n) {
    unsigned int i;

    if (n == 0) return NAN;

    if (0 < x && x < 1) {

        double vys = 0;
        double moc = 1;

        x = 1 - x;

        for (i = 1; i < n+1; i++) {
            moc *= x;
            vys -= moc / i;
        }

        return vys;

    } else if (x >= 1) {

        double vys = 0;
        double moc = 1;

        for (i = 1; i < n+1; i++) {
            moc *= ((x - 1) / x);
            vys += moc / i;
        }

        return vys;
    } else if (x == 0) return -INFINITY;

    return NAN;
}

//Vypocet CFRAC logaritmu pre vystupnu hodnotu X a pocet interacii N
double cfrac_log(double x, unsigned int n) {
    unsigned int i;

    double vys;
    double pred = 0;    
    
    if (n == 0) return NAN;
    
    if (x == 0) return -INFINITY;
    else if (x < 0) return NAN;

    x = (x-1) / (x+1);

    for (i = n - 1; i > 0; i--) {
        pred = (i*i*x*x) / (2*i + 1 - pred);
    }

    vys = 2*x / (1 - pred);
    return vys;
}
