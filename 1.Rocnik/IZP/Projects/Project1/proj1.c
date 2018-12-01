/* Projekt 1 IZP, 2015/2016
 * Autor: Róbert Kolcún, FIT
 * <xkolcu00@stud.fit.vutbr.cz>
*/

#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <stdlib.h>

//Funcia zisti ci dane znaky zo vstupu sa nachadzaju v ASCII
int IsASCII(char *v, int dlzka)
{
  int i;

  for (i = 0; i < dlzka; i++) {
    if (isprint(v[i]) == 0) {
      printf("ERROR invalid input\n");
      exit (0);
    }
  }
  return 1;
}


//Overi ci Input je cislo
int IsNumber(char *v, int dlzka)
{
  int i;

  for (i = 0; i < dlzka; i++) {
    if (v[i] > 57 || v[i] < 48){
	//printf("%c\n", v[i]);
	return 0;
    }
  }
  return 1;
}


int IsPom(char *v)
{
  if (v[4] == '-' && v[7] == '-') return 1;
  else return 0;
}

//Overenie ci den a mesiac su cisla
int IsDayMonNumber(int zac, int count, char *v)
{
  int i;

  for (i = 0; i < count;i++) {
    if (v[zac + i] > 57 || v[zac + i] < 48) return 0;
  }
  return 1;
}

//Funkcia pomocou MkTime, overi datum a zisti den v tyzdni
void SetDate(char *v)
{
  struct tm myTime;
  char buffer[20];

  char vys[11];
  int i;
  int isValidDate = 1;

  int rok;
  int mesiac;

  for (i = 0; i < 10; i++) {
    if (v[i] == '-') vys[i] = ' ';
	else vys[i] = v[i];
  }
  vys[10] = '\0';
  //printf("%s\n", vys);
  sscanf(vys, "%d %d %d", &rok, &mesiac, &myTime.tm_mday);

  //printf("%d\n", r);

  if (rok < 1900 || myTime.tm_mday > 31 || myTime.tm_mday < 1 || mesiac < 1 || mesiac > 12)
	isValidDate = 0;

  myTime.tm_year = rok - 1900;
  myTime.tm_mon = mesiac - 1;
  myTime.tm_hour = 0;
  myTime.tm_min = 0;
  myTime.tm_sec = 1;
  myTime.tm_isdst = -1;

  if (mktime(&myTime) == -1 || isValidDate == 0)
	printf("word: %s", v);
  else {
    strftime(buffer, sizeof(buffer), "%a %F", &myTime);
    printf("date: %s", buffer);
  }
}

//Zistenie dlzky retazka zo vstupu
int StringLength(char *v)
{
  int i = 0;

  while (v[i] != '\0') {
    i++;
    if (i > 100) {
      printf("ERROR invalid input\n");
      exit(0);
    }
  }

  return i;
}

//Funkcia overy si dany Input ma tvar datumu DDDD-DD-DD
int IsDate(char *v)
{
  if (StringLength(v) == 10 && IsNumber(v, 4) && IsPom(v)
	&& IsDayMonNumber(5, 2, v) && IsDayMonNumber(8, 2, v)) return 1;
  else return 0;
}


//Funkcia vymaze nuly na zaciatku cisla
int DeleteZeros(char *v) {
  int i;
  char vys[StringLength(v)];
  int zeroCount = 0;
  int dlzka = StringLength(v);

  for (i = 0; i < dlzka; i++) {
    if (v[i] == 48) zeroCount++;
    else break;
  }

  //printf("%d\n", zeroCount);

  if (zeroCount != 0) {
    for (i = zeroCount; i < dlzka; i++) {
	vys[i - zeroCount] = v[i];
	//printf("C");
    }
    vys[dlzka - zeroCount] = '\0';
    if (dlzka != zeroCount) {
      printf("number: %s", vys);
      return 1;
    }
    else {
      printf("number: 0");
      return 2;
    }
  } else {
      printf("number: %s", v);
      return 0;
  }
}


//Zisti ci dany retazec je palindrom
int IsPalindrom(char *v)
{
  int i;
  int dlzka = StringLength(v);
  int dlzkaCyklu = dlzka / 2;
  
  //printf(" %d %d", dlzka, dlzkaCyklu);
  
  for (i = 0; i < dlzkaCyklu; i++) {
    if (v[i] != v[dlzka - i - 1]) return 0;
  }
  return 1;
}

//Overi ze zanadene cislo je prvocislo
int IsPrime(char *v)
{
  int pocet=0;
  int i, cis;

  sscanf(v, "%d", &cis);

  for (i = 1; i < cis+1; i++)
  {
    if (cis % i == 0) pocet++;
  }

  if (pocet == 2) return 1;
	else return 0;
}

int main(int argc, char *argv[])
{
  char s[101];

  (void) argv;

  if (argc > 1) {
    printf("Autor: Robert Kolcun, xkolcu00\nProgram na rozoznanie vstupu (date, number, string)\na urcenie prvocisla a palindromu.\n");
    return 0;
  }

  while (scanf("%s", s) != EOF) //Vypis vylednych hodnot
  { 
    if (IsASCII(s, StringLength(s))) {
      if (IsNumber(s, StringLength(s))) {
	if (DeleteZeros(s) != 2)
	  if (IsPrime(s)) printf(" (prime)");
	printf("\n");
      } else {
	if (IsDate(s)) SetDate(s);
	  else {
	    printf("word: %s", s);
	    if (IsPalindrom(s)) printf(" (palindrome)");
	  }
	printf("\n");
      }
    }
  //  printf("%s %d\n", s,StringLength(s));
  }
  return 0;
}
