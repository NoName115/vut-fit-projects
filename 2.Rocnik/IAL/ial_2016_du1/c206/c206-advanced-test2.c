/* Soubor:   c206-bigtest.c
 * Datum:    16.10.2009
 * Autor:    Petr Novohradský, xnovoh01
 *           xnovoh01@stud.fit.vutbr.cz
 *           Fakulta informaèních technologií
 *           Vysoké uèení technické v Brnì
 * Projekt:  Roz¹íøené testování úkolu do pøedmìtu IAL, zima 2009
 * Poznámky: Encoding ISO-8859-2
 *           Testuje implementaci lineárního dvousmìrnì vázaného seznamu
 *           Nehlídá pøíznak solved -> pøedpokládá ¾e u¾ jsou hotové v¹echny funkce
 */

// práce se vstupem/výstupem
#include <stdio.h>

// obecné funkce jazyka C
#include <stdlib.h>

// typ bool, konstanty true, false
#include <stdbool.h>

//implementovaný seznam
#include "c206.h"

/* Vypí¹e seznam prvkù a ovìøí v¹echny ukazatele
 * Nevyu¾ívá ji¾ implementované funkce
 */
void VypisPrvku(tDLList Seznam)
{
  if(errflg)//kontrola chyb knihovny
  {
    printf("Poslední operace hlásí chybu.\n");
    errflg = false;
  }
  printf("-------------------\n");//Zaèátek výpisu
  if(Seznam.First == NULL)//Seznam je prázdný
  {
    printf("Seznam je prázdný, ukazatele jsou ");
    if((Seznam.Last == NULL)&&(Seznam.Act == NULL))
      printf("správnì.\n");
    else
    {
      printf("¹patnì.\n");
      printf("V¹echny ukazatele ve struktuøe tDLList musí být NULL.\n");
    }
  }
  else//V seznamu nìco je
  {
    bool AktivniNalezen = false;
    printf("Hodnota Ovìøení Aktivita\n");//Hlavièka výpisu
    tDLElemPtr Prvek, Predchozi;
    Predchozi = NULL;
    Prvek = Seznam.First;
    while(Prvek != NULL)//Projde celý seznam
    {
      printf("%-8d", Prvek->data);//Tisk hodnoty prvku
      if(Prvek->lptr == Predchozi)//ovìøí platnost ukazatele na pøedchozí prvek
        printf("OK      ");
      else
        printf("ERR lptr");
      if(Prvek == Seznam.Act)//Ovìøí aktivitu
      {
        AktivniNalezen = true;
        printf("Aktivní prvek");
      }
      printf("\n");
      Predchozi = Prvek;
      Prvek = Prvek->rptr;
    }
    if(Seznam.Last != Predchozi)
      printf("Ukazatel na poslední prvek je ¹patnì!\n");
    printf("Ukazatel na aktivní prvek ");
    if((AktivniNalezen)||(Seznam.Act == NULL))
      printf("OK\n");
    else
      printf("ukazuje mimo seznam!\n");
  }
  printf("-------------------\n");//Konec výpisu
}

int main(void)
{
  int pocitadlo = 1;//poèítadlo, plnit se bude v¾dy jinou hodnotou
  int vysledek;//Promìnná na výsledek operace
  tDLList Seznam;
  Seznam.Act = (tDLElemPtr)1;//inicializace struktury na jiné ne¾ standard. hodnoty
  Seznam.First = (tDLElemPtr)1;
  Seznam.Last = (tDLElemPtr)1;
  
  printf("\n*******************\n[TEST 1]\n");
  printf("DLInitList\n");
  DLInitList(&Seznam);
  VypisPrvku(Seznam);
  
  printf("\n*******************\n[TEST 2]\n");
  printf("DLDisposeList - pro prázdný seznam\n");
  DLDisposeList(&Seznam);
  VypisPrvku(Seznam);
  
  printf("\n*******************\n[TEST 3]\n");
  printf("DLFirst - pro prázdný seznam\n");
  DLFirst(&Seznam);
  VypisPrvku(Seznam);
  
  printf("\n*******************\n[TEST 4]\n");
  printf("DLLast - pro prázdný seznam\n");
  DLLast(&Seznam);
  VypisPrvku(Seznam);
  
  printf("\n*******************\n[TEST 5]\n");
  printf("DLCopyFirst - pro prázdný seznam\n");
  vysledek = -1;
  DLCopyFirst(&Seznam, &vysledek);
  printf("Výsledek: %d\n", vysledek);
  VypisPrvku(Seznam);
  
  printf("\n*******************\n[TEST 6]\n");
  printf("DLCopyLast - pro prázdný seznam\n");
  vysledek = -1;
  DLCopyLast(&Seznam, &vysledek);
  printf("Výsledek: %d\n", vysledek);
  VypisPrvku(Seznam);
  
  printf("\n*******************\n[TEST 7]\n");
  printf("DLDeleteFirst - pro prázdný seznam\n");
  DLDeleteFirst(&Seznam);
  VypisPrvku(Seznam);
  
  printf("\n*******************\n[TEST 8]\n");
  printf("DLDeleteLast - pro prázdný seznam\n");
  DLDeleteLast(&Seznam);
  VypisPrvku(Seznam);
  
  printf("\n*******************\n[TEST 9]\n");
  printf("DLInsertFirst - pro prázdný seznam\n");
  DLInsertFirst(&Seznam, pocitadlo++);
  VypisPrvku(Seznam);
  
  printf("\n*******************\n[TEST 10]\n");
  printf("DLInsertFirst - pro neprázdný seznam\n");
  DLInsertFirst(&Seznam, pocitadlo++);
  VypisPrvku(Seznam);
  
  printf("\n*******************\n[TEST 11]\n");
  printf("DLInsertLast - pro neprázdný seznam\n");
  DLInsertLast(&Seznam, pocitadlo++);
  VypisPrvku(Seznam);
  
  printf("\n*******************\n[TEST 12]\n");
  printf("DLCopyFirst - pro neprázdný seznam\n");
  vysledek = -1;
  DLCopyFirst(&Seznam, &vysledek);
  printf("Výsledek: %d\n", vysledek);
  VypisPrvku(Seznam);
  
  printf("\n*******************\n[TEST 13]\n");
  printf("DLCopyLast - pro neprázdný seznam\n");
  vysledek = -1;
  DLCopyLast(&Seznam, &vysledek);
  printf("Výsledek: %d\n", vysledek);
  VypisPrvku(Seznam);
  
  printf("\n*******************\n[TEST 14]\n");
  printf("DLCopy - pro neaktivní seznam\n");
  vysledek = -1;
  DLCopy(&Seznam, &vysledek);
  printf("Výsledek: %d\n", vysledek);
  VypisPrvku(Seznam);
  
  printf("\n*******************\n[TEST 15]\n");
  printf("DLActualize - pro neaktivní seznam\n");
  DLActualize(&Seznam, pocitadlo++);
  VypisPrvku(Seznam);
  
  printf("\n*******************\n[TEST 16]\n");
  printf("DLSucc - pro neaktivní seznam\n");
  DLSucc(&Seznam);
  VypisPrvku(Seznam);
  
  printf("\n*******************\n[TEST 17]\n");
  printf("DLPred - pro neaktivní seznam\n");
  DLPred(&Seznam);
  VypisPrvku(Seznam);
  
  printf("\n*******************\n[TEST 18]\n");
  printf("DLPostDelete - pro neaktivní seznam\n");
  DLPostDelete(&Seznam);
  VypisPrvku(Seznam);
  
  printf("\n*******************\n[TEST 19]\n");
  printf("DLPreDelete - pro neaktivní seznam\n");
  DLPreDelete(&Seznam);
  VypisPrvku(Seznam);
  
  printf("\n*******************\n[TEST 20]\n");
  printf("DLPostInsert - pro neaktivní seznam\n");
  DLPostInsert(&Seznam, pocitadlo++);
  VypisPrvku(Seznam);
  
  printf("\n*******************\n[TEST 21]\n");
  printf("DLPreInsert - pro neaktivní seznam\n");
  DLPreInsert(&Seznam, pocitadlo++);
  VypisPrvku(Seznam);
  
  printf("\n*******************\n[TEST 22]\n");
  printf("DLFirst - pro neprázdný seznam\n");
  DLFirst(&Seznam);
  VypisPrvku(Seznam);
  
  printf("\n*******************\n[TEST 23]\n");
  printf("DLCopy - pro aktivní seznam\n");
  vysledek = -1;
  DLCopy(&Seznam, &vysledek);
  printf("Výsledek: %d\n", vysledek);
  VypisPrvku(Seznam);
  
  printf("\n*******************\n[TEST 24]\n");
  printf("DLActualize - pro aktivní seznam\n");
  DLActualize(&Seznam, pocitadlo++);
  VypisPrvku(Seznam);
  
  printf("\n*******************\n[TEST 25]\n");
  printf("DLSucc - pro aktivní seznam\n");
  DLSucc(&Seznam);
  VypisPrvku(Seznam);
  
  printf("\n*******************\n[TEST 26]\n");
  printf("DLPred - pro aktivní seznam\n");
  DLPred(&Seznam);
  VypisPrvku(Seznam);
  
  printf("\n*******************\n[TEST 27]\n");
  printf("DLPred - pro aktivní první prvek seznamu\n");
  DLPred(&Seznam);
  VypisPrvku(Seznam);
  
  printf("\n*******************\n[TEST 28]\n");
  printf("DLLast - pro neprázdný seznam\n");
  DLLast(&Seznam);
  VypisPrvku(Seznam);
  
  printf("\n*******************\n[TEST 29]\n");
  printf("DLSucc - pro aktivní poslední prvek seznamu\n");
  DLSucc(&Seznam);
  VypisPrvku(Seznam);
  
  printf("\nPøesouvám aktivitu seznamu na poslední prvek...\n");
  DLLast(&Seznam);
  
  printf("\n*******************\n[TEST 30]\n");
  printf("DLPostInsert - pro aktivní poslední prvek seznamu\n");
  DLPostInsert(&Seznam, pocitadlo++);
  VypisPrvku(Seznam);

  printf("\n*******************\n[TEST 31]\n");
  printf("DLPostInsert - uprostøed seznamu\n");
  DLPostInsert(&Seznam, pocitadlo++);
  VypisPrvku(Seznam);
  
  printf("\nPøesouvám aktivitu seznamu na první prvek...\n");
  DLFirst(&Seznam);

  printf("\n*******************\n[TEST 32]\n");
  printf("DLPreInsert - pro aktivní první prvek seznamu\n");
  DLPreInsert(&Seznam, pocitadlo++);
  VypisPrvku(Seznam);

  printf("\n*******************\n[TEST 33]\n");
  printf("DLPreInsert - uprostøed seznamu\n");
  DLPreInsert(&Seznam, pocitadlo++);
  VypisPrvku(Seznam);

  printf("\nPøesouvám aktivitu seznamu na první prvek...\n");
  DLFirst(&Seznam);
  
  printf("\n*******************\n[TEST 34]\n");
  printf("DLDeleteFirst - pro neprázdný seznam, ma¾e se aktivní prvek\n");
  DLDeleteFirst(&Seznam);
  VypisPrvku(Seznam);
  
  printf("\nPøesouvám aktivitu seznamu na poslední prvek...\n");
  DLLast(&Seznam);
  
  printf("\n*******************\n[TEST 35]\n");
  printf("DLDeleteLast - pro neprázdný seznam, ma¾e se aktivní prvek\n");
  DLDeleteLast(&Seznam);
  VypisPrvku(Seznam);
  
  printf("\nPøesouvám aktivitu seznamu na druhý prvek...\n");
  DLFirst(&Seznam);
  DLSucc(&Seznam);
  
  printf("\n*******************\n[TEST 36]\n");
  printf("DLPreDelete - ma¾e se první prvek seznamu\n");
  DLPreDelete(&Seznam);
  VypisPrvku(Seznam);
  
  printf("\n*******************\n[TEST 37]\n");
  printf("DLPreDelete - aktivní první prvek seznamu\n");
  DLPreDelete(&Seznam);
  VypisPrvku(Seznam);
  
  printf("\nPøesouvám aktivitu seznamu na druhý prvek od konce...\n");
  DLLast(&Seznam);
  DLPred(&Seznam);
  
  printf("\n*******************\n[TEST 38]\n");
  printf("DLPostDelete - ma¾e se poslední prvek seznamu\n");
  DLPostDelete(&Seznam);
  VypisPrvku(Seznam);
  
  printf("\n*******************\n[TEST 39]\n");
  printf("DLPostDelete - aktivní poslední prvek seznamu\n");
  DLPostDelete(&Seznam);
  VypisPrvku(Seznam);
  
  printf("\n*******************\n[TEST 40]\n");
  printf("DLPreDelete - uprostøed seznamu\n");
  DLPreDelete(&Seznam);
  VypisPrvku(Seznam);
  
  printf("\nPøidávám prvek na konec seznamu...\n");
  DLInsertLast(&Seznam, pocitadlo++);
  printf("Pøesouvám aktivitu seznamu na první prvek...\n");
  DLFirst(&Seznam);

  printf("\n*******************\n[TEST 41]\n");
  printf("DLPostDelete - uprostøed seznamu\n");
  DLPostDelete(&Seznam);
  VypisPrvku(Seznam);
  
  printf("\n*******************\n[TEST 42]\n");
  printf("DLDisposeList - pro neprázdný seznam\n");
  DLDisposeList(&Seznam);
  VypisPrvku(Seznam);

  printf("\n*******************\n[TEST 43]\n");
  printf("DLInsertLast - pro prázdný seznam\n");
  DLInsertLast(&Seznam, pocitadlo++);
  VypisPrvku(Seznam);

  printf("\nPøesouvám aktivitu seznamu na první prvek...\n");
  DLFirst(&Seznam);

  printf("\n*******************\n[TEST 44]\n");
  printf("DLDeleteFirst - mazání posledního prvku, prvek je aktivní\n");
  DLDeleteFirst(&Seznam);
  VypisPrvku(Seznam);

  printf("\nVkládám do seznamu prvek a nastavuji mu aktivitu...\n");
  DLInsertFirst(&Seznam, pocitadlo++);
  DLFirst(&Seznam);
  
  printf("\n*******************\n[TEST 45]\n");
  printf("DLDeleteLast - mazání posledního prvku\n");
  DLDeleteLast(&Seznam);
  VypisPrvku(Seznam);
}
