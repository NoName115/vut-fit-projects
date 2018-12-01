	
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Bohuslav Křena, říjen 2016
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu, 
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek, 
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku, 
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu, 
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem, 
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu, 
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
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

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/

	L->First = NULL;
	L->Act = NULL;
	L->Last = NULL;
    
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/
	
	//Ulozim si ukazoval na posledny prvok
	tDLElemPtr temp = L->Last;

	//Vynulujem hodnoty listu L (ako pri inicializacii)
	L->Act = NULL;
	L->First = NULL;

	while (temp != NULL)
	{
		tDLElemPtr tempNext = temp->lptr;
		free(temp);
		temp = tempNext;
	}

	L->Last = NULL;
	
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	
	//Alokuje miesto pre novy prvok
	tDLElemPtr newElem = malloc(sizeof(struct tDLElem));

	//Alokacia sa nepodarila
	if (newElem == NULL)
	{
		DLError();
		return;
	}

	newElem->rptr = L->First;
	newElem->lptr = NULL;
	newElem->data = val;

	if (L->First != NULL)
	{
		L->First->lptr = newElem;
	}
	else
	{
		L->Last = newElem;
	}

	L->First = newElem;

}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

	//Alokuje miesto pre novy prvok
	tDLElemPtr newElem = malloc(sizeof(struct tDLElem));

	//Alokacia sa nepodarila
	if (newElem == NULL)
	{
		DLError();
		return;
	}

	newElem->rptr = NULL;
	newElem->lptr = L->Last;
	newElem->data = val;

	if (L->Last != NULL)
	{
		L->Last->rptr = newElem;
	}
	else
	{
		L->First = newElem;
	}

	L->Last = newElem;

}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/

	L->Act = L->First;
	
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/

	L->Act = L->Last;
	
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

	if (L->First == NULL)
	{
		DLError();
		return;
	}

	*val = L->First->data;
	
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

	if (L->First == NULL)
	{
		DLError();
		return;
	}
	
	*val = L->Last->data;

}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/

	//Zoznam je prazdny
	if (L->First == NULL)
	{
		return;
	}

	//Aktivny prvok je prvy prvok
	if (L->Act == L->First)
	{
		L->Act = NULL;
	}

	//Do temp si ulozim nasledovnika prveho prvku
	tDLElemPtr temp = L->First->rptr;

	//temp prechadzajuci prvok sa bude rusit
	if (temp != NULL)
	{
		temp->lptr = NULL;
	}

	//Uvolim prvy prvok
	free(L->First);

	//Novy prvy prvok je temp
	L->First = temp;

	if (L->First == NULL)
	{
		L->Last = NULL;
	}
	
}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/

	//Zoznam je prazdny
	if (L->First == NULL)
	{
		return;
	}

	//Aktivny prvok je posledny prvok
	if (L->Act == L->Last)
	{
		L->Act = NULL;
	}

	//Ulozim si predchadzajuci prvok od last
	tDLElemPtr temp = L->Last->lptr;

	//Nasledujuci prvok sa bude rusit
	if (temp != NULL)
	{
		temp->rptr = NULL;
	}
	else
	{
		L->First = NULL;
	}
	
	//Uvolnik posledny prvok zoznamu
	free(L->Last);

	//Posledny prvok zoznamu je temp
	L->Last = temp;

}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/

	//Zoznam je neaktivny alebo aktivny prvok je posledny prvok
	if (L->Act == NULL || L->Act == L->Last)
	{
		return;
	}

	//Do temp si ulozim nasledujuci prvok aktivneho
	tDLElemPtr temp = L->Act->rptr;

	if (temp->rptr != NULL)
	{
		//Nasledujuci prvok temp ukazuje na aktivny
		temp->rptr->lptr = L->Act;

		//Nasledujuci prvok aktivneho je prvok za temp
		L->Act->rptr = temp->rptr;
	}
	else
	{
		L->Act->rptr = temp->rptr;
		L->Last = L->Act;
	}

	//Uvolnim temp
	free(temp);
	
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/

	//Zoznam je neaktivny alebo aktivny prvok je prvy prvok
	if (L->Act == NULL || L->Act == L->First)
	{
		return;
	}

	tDLElemPtr temp = L->Act->lptr;

	L->Act->lptr = L->Act->lptr->lptr;

	if (temp->lptr != NULL)
	{
		temp->lptr->rptr = L->Act;
	}
	else
	{
		L->First = L->Act;
	}

	free(temp);
	
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

	//Zoznam nieje aktivny
	if (L->Act == NULL)
	{
		return;
	}

	//Alokujem pamat pre novy prvok
	tDLElemPtr newElem = malloc(sizeof(struct tDLElem));

	//Alokacia sa nepodarila
	if (newElem == NULL)
	{
		DLError();
		return;
	}

	//Do temp si ulozim nasledujuci prvok aktivneho
	tDLElemPtr temp = L->Act->rptr;

	//Vlozim novy prvok do zoznamu
	L->Act->rptr = newElem;

	//Nastavim hodnoty novemu elementu
	newElem->lptr = L->Act;
	newElem->rptr = temp;
	newElem->data = val;

	//temp predchadzuji ukazuje na novy prvok
	if (temp != NULL)
	{
		temp->lptr = newElem;
	}
	else
	{
		L->Last = newElem;
	}
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

	//Zoznam nieje aktivny
	if (L->Act == NULL)
	{
		return;
	}

	//Alokujem pamat pre novy prvok
	tDLElemPtr newElem = malloc(sizeof(struct tDLElem));

	//Alokacia sa nepodarila
	if (newElem == NULL)
	{
		DLError();
		return;
	}
	
	//Do temp si ulozim predchadzajuci prvok aktivneho
	tDLElemPtr temp = L->Act->lptr;

	//Vlozim novy prvok do zoznamu
	L->Act->lptr = newElem;

	//Nastavim hodnoty novemu prvku
	newElem->rptr = L->Act;
	newElem->lptr = temp;
	newElem->data = val;

	if (temp != NULL)
	{
		//temp nasledujuci ukazuje na novy prvok
		temp->rptr = newElem;
	}
	else
	{
		L->First = newElem;
	}
	
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
		
	//Zoznam nieje aktivny
	if (L->Act == NULL)
	{
		DLError();
		return;
	}
	
	*val = L->Act->data;

}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
	
	//Zoznam nieje aktivny
	if (L->Act == NULL)
	{
		return;
	}

	L->Act->data = val;
	
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/

	//Zoznam nieje aktivny
	if (L->Act == NULL)
	{
		return;
	}

	L->Act = L->Act->rptr;
	
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
	
	//Zoznam nieje aktivny
	if (L->Act == NULL)
	{
		return;
	}

	L->Act = L->Act->lptr;
	
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/

	return L->Act == NULL ? 0 : 1;
	
}

/* Konec c206.c*/
