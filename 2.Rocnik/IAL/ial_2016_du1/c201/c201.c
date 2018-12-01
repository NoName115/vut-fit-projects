
/* c201.c *********************************************************************}
{* Téma: Jednosměrný lineární seznam
**
**                     Návrh a referenční implementace: Petr Přikryl, říjen 1994
**                                          Úpravy: Andrea Němcová listopad 1996
**                                                   Petr Přikryl, listopad 1997
**                                Přepracované zadání: Petr Přikryl, březen 1998
**                                  Přepis do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Bohuslav Křena, říjen 2016
**
** Implementujte abstraktní datový typ jednosměrný lineární seznam.
** Užitečným obsahem prvku seznamu je celé číslo typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou typu tList.
** Definici konstant a typů naleznete v hlavičkovém souboru c201.h.
** 
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ tList:
**
**      InitList ...... inicializace seznamu před prvním použitím,
**      DisposeList ... zrušení všech prvků seznamu,
**      InsertFirst ... vložení prvku na začátek seznamu,
**      First ......... nastavení aktivity na první prvek,
**      CopyFirst ..... vrací hodnotu prvního prvku,
**      DeleteFirst ... zruší první prvek seznamu,
**      PostDelete .... ruší prvek za aktivním prvkem,
**      PostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      Copy .......... vrací hodnotu aktivního prvku,
**      Actualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      Succ .......... posune aktivitu na další prvek seznamu,
**      Active ........ zjišťuje aktivitu seznamu.
**
** Při implementaci funkcí nevolejte žádnou z funkcí implementovaných v rámci
** tohoto příkladu, není-li u dané funkce explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c201.h"

int solved;
int errflg;

void Error() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;                      /* globální proměnná -- příznak chyby */
}

void InitList (tList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/

	L->First = NULL;
	L->Act = NULL;

}

void DisposeList (tList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam L do stavu, v jakém se nacházel
** po inicializaci. Veškerá paměť používaná prvky seznamu L bude korektně
** uvolněna voláním operace free.
***/
	
	//Ulozim si ukazoval na prvy prvok
	tElemPtr temp = L->First;

	//Vynulujem hodnoty listu L (ako pri inicializacii)
	L->Act = NULL;
	L->First = NULL;

	//Postupne uvolnulem pamat po prvkov
	while (temp != NULL)
	{
		tElemPtr tempNext = temp->ptr;
		free(temp);
		temp = tempNext;
	}

}

void InsertFirst (tList *L, int val) {
/*
** Vloží prvek s hodnotou val na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci Error().
**/
    
    //Alokujem pamat pre novy element
	tElemPtr elem = malloc(sizeof(struct tElem));

	//Alokacia sa nepodarila
	if (elem == NULL)
	{
		Error();
		return;
	}

	//Ulozim hodnotu val do elementu
	//do ukazovatela na prvok vlozim ukazovatel na prvy prvok z zoznamu
	elem->ptr = L->First;
	elem->data = val;

	//Prvy prvok je novy element
	L->First = elem;

}

void First (tList *L) {
/*
** Nastaví aktivitu seznamu L na jeho první prvek.
** Funkci implementujte jako jediný příkaz, aniž byste testovali,
** zda je seznam L prázdný.
**/
	
	L->Act = L->First;

}

void CopyFirst (tList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci Error().
**/
	
	if (L->First == NULL)
	{
		Error();
		return;
	}

	//Vlozim nove val data do prve elementu
	*val = L->First->data;
	
}

void DeleteFirst (tList *L) {
/*
** Zruší první prvek seznamu L a uvolní jím používanou paměť.
** Pokud byl rušený prvek aktivní, aktivita seznamu se ztrácí.
** Pokud byl seznam L prázdný, nic se neděje.
**/
	
	//Zoznam je prazdny
	if (L->First == NULL)
	{
		return;
	}

	//Aktivny prvok bol prvok ktory sa rusi(prvy)
	if (L->First == L->Act)
	{
		L->Act = NULL;
	}

	//Ulozim si ukazovatel na druhy prvok
	tElemPtr temp = L->First->ptr;

	//Uvolni pamat prvemu prvku
	free(L->First);

	//Nastavim aby first ukatoval na uz teraz prvy prvok
	L->First = temp;

}	

void PostDelete (tList *L) {
/* 
** Zruší prvek seznamu L za aktivním prvkem a uvolní jím používanou paměť.
** Pokud není seznam L aktivní nebo pokud je aktivní poslední prvek seznamu L,
** nic se neděje.
**/
	
	//Zoznam nieje aktivny, alebo posledny prvok je aktivny
	if (L->Act == NULL || L->Act->ptr == NULL)
	{
		return;
	}

	//Ulozim si ukazoval na prvok za aktivnym prvok
	tElemPtr temp = L->Act->ptr->ptr;

	//Uvolnim pamat prvku na ktory ukazuje aktivny prvok
	free(L->Act->ptr);

	//Activny prvok teraz ukazuje temp
	L->Act->ptr = temp;
	
}

void PostInsert (tList *L, int val) {
/*
** Vloží prvek s hodnotou val za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje!
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** zavolá funkci Error().
**/

	//Zoznam je neaktivny
	if (L->Act == NULL)
	{
		return;
	}

	//Alokujem pamat pre novy element
	tElemPtr newElem = malloc(sizeof(struct tElem));

	//Nedostatok pamata --> Error
	if (newElem == NULL)
	{
		Error();
		return;
	}

	//Ulozim data do noveho prvku
	newElem->data = val;

	//Pridelim ukazovatele novemu elementu a aktivnemu prvku
	newElem->ptr = L->Act->ptr;
	L->Act->ptr = newElem;
	
}

void Copy (tList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam není aktivní, zavolá funkci Error().
**/

	if (L->Act == NULL)
	{
		Error();
		return;
	}

	*val = L->Act->data;

}

void Actualize (tList *L, int val) {
/*
** Přepíše data aktivního prvku seznamu L hodnotou val.
** Pokud seznam L není aktivní, nedělá nic!
**/

	if (L->Act == NULL)
	{
		return;
	}

	L->Act->data = val;

}

void Succ (tList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Všimněte si, že touto operací se může aktivní seznam stát neaktivním.
** Pokud není předaný seznam L aktivní, nedělá funkce nic.
**/

	if (L->Act == NULL)
	{
		return;
	}

	L->Act = L->Act->ptr;

}

int Active (tList *L) {		
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Tuto funkci je vhodné implementovat jedním příkazem return. 
**/
	
	return L->Act == NULL ? 0 : 1;

}

/* Konec c201.c */
