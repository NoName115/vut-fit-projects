/* Cvicenia IZP-CV_7, 2015/2016
 * Autor: Róbert Kolcún, FIT
 * <xkolcu00@stud.fit.vutbr.cz>
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
  int id;
  char *name;
} Object;

typedef struct item {
  Object data;
  struct item *next;
} Item;

typedef struct {
  Item *first;
} List;

/**
 * Inicializace seznamu. Vytvoøí prázdný seznam.
 */
List list_ctor() {
    ///DONE
    List myList;

    myList.first = NULL;

    return myList;
}

/**
 * Inicializace položky seznamu. Z objektu vytvoøí položku bez následníka.
 */
Item item_ctor(Object data) {
    ///DONE
    Item myItem;

    myItem.data = data; //Vlozi udaje objectu DATA z argumentu
    myItem.next = NULL;

    return myItem;
}

/**
 * Vloží položku na zaèátek seznamu.
 */
void list_insert_first(List *list, Item *i) {
    ///DONE
    list->first = i;
    list->first->next = NULL;
}

/**
 * Vrátí poèet položek seznamu.
 */
unsigned list_count(List *list) {
    ///DONE
    int poc = 0;

    Item *l;
    l = list->first;

    while (l != NULL) {
        l = l->next;
        poc++;
    }

    return poc;
}

/**
 * Vrátí true, pokud je seznam prázdný.
 */
bool list_empty(List *list) {
    ///DONE
    if (list->first == NULL)
        return true;

    return false;
}

/**
 * Najde položku seznamu s nejmenším identifikátorem. Vrací NULL, pokud je
 * seznam prázdný.
 */
Item *list_find_minid(List *list) {
    ///DONE
    int minID = 2000;
    Item *myItem;

    if (list_empty(list))
        return NULL;

    Item *l;
    l = list->first;

    while (l != NULL) {
        if (l->data.id < minID) {
            minID = l->data.id;
            myItem = l;

        }
        l = l->next;
    }

    return myItem;
}

/**
 * Najde položku seznamu s odpovídajícím jménem objektu. Vrací NULL, pokud
 * taková položka v seznamu není.
 */
Item *list_find_name(List *list, char *name){
    ///DONE
    Item *myItem = NULL;

    if (list_empty(list))
        return NULL;

    Item *l;
    l = list->first;

    while (l != NULL) {
        if (strcmp(l->data.name, name) == 0) {
            myItem = l;
        }
        l = l->next;
    }

    return myItem;
}

int main() {
    //Vytvori a inicializuje zoznam
    List myList;
    myList = list_ctor();

    //Vytvori Object Udaje
    Object udaje;
    udaje.id = 5;
    udaje.name = "Robo";
    //Vrati Item do ktoreho nahral Object Udaje
    Item newItem = item_ctor(udaje);

    //Item vlozi na zaciatok zoznamu
    list_insert_first(&myList, &newItem);

    //Nove udaje
    udaje.id = 2;
    udaje.name = "Edo";

    //Vytvori novy ITEM
    Item znovaItem = item_ctor(udaje);
    //Vlozi novy item do zoznamu
    myList.first->next = &znovaItem;
    myList.first->next->next = NULL;

    //Zisti pocet Item v zozname
    int pocet = list_count(&myList);
    printf("POCET: %d\n", pocet);

    Item *wordItem = list_find_name(&myList, "Robo"); //list_find_minid(&myList);
    if (wordItem != NULL)
        printf("WORD: %s\n", wordItem->data.name);

    Item *minItem = list_find_minid(&myList);
    printf("MIN: %d\n", minItem->data.id);


    if (list_empty(&myList))
        printf("EMPTY");
    else {
        printf("%d - %s\n", myList.first->data.id, myList.first->data.name);
        printf("%d - %s\n", myList.first->next->data.id, myList.first->next->data.name);
    }

    return 0;
}

