//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - priority queue code
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author ROBERT
 * 
 * @brief Implementace metod tridy prioritni fronty.
 */

#include <stdlib.h>
#include <stdio.h>

#include "tdd_code.h"

//============================================================================//
// ** ZDE DOPLNTE IMPLEMENTACI **
//
// Zde doplnte implementaci verejneho rozhrani prioritni fronty (Priority Queue)
// 1. Verejne rozhrani fronty specifikovane v: tdd_code.h (sekce "public:")
//    - Konstruktor (PriorityQueue()), Destruktor (~PriorityQueue())
//    - Metody Insert/Remove/Find a GetHead
//    - Pripadne vase metody definovane v tdd_code.h (sekce "protected:")
//
// Cilem je dosahnout plne funkcni implementace prioritni fronty implementovane
// pomoci tzv. "double-linked list", ktera bude splnovat dodane testy 
// (tdd_tests.cpp).
//============================================================================//

PriorityQueue::PriorityQueue()
{

}

PriorityQueue::~PriorityQueue()
{
	struct Element_t *tempElement = root;

	while (tempElement != NULL)
	{
		struct Element_t *nextElement = tempElement->pNext;
		free(tempElement);
		tempElement = nextElement;
	}
}

void PriorityQueue::Insert(int value)
{
	struct Element_t *newElement = (struct Element_t *) malloc(sizeof(struct Element_t));
	newElement->value = value;

	if (root == NULL)
	{
		root = newElement;
		newElement->pNext = NULL;
		newElement->pPrev = NULL;
	}
	else
	{
		struct Element_t *prevElement = NULL;
		struct Element_t *tempElement = root;
		while (tempElement != NULL)
		{
			if (tempElement->value >= value)
			{
				break;
			}

			prevElement = tempElement;
			tempElement = tempElement->pNext;
		}

		if (tempElement != NULL)
		{
			// == NULL is root
			if (tempElement->pPrev != NULL)
			{
				tempElement->pPrev->pNext = newElement;
			}
			else
			{
				root = newElement;
			}

			tempElement->pPrev = newElement;
		}
		else
		{
			prevElement->pNext = newElement;
		}

		newElement->pPrev = prevElement;
		newElement->pNext = tempElement;
	}
}

bool PriorityQueue::Remove(int value)
{
	struct Element_t *tempElement = root;

	while (tempElement != NULL)
	{
		if (tempElement->value == value)
		{
			if (tempElement->pNext != NULL)
			{
				tempElement->pNext->pPrev = tempElement->pPrev;
			}

			if (tempElement->pPrev != NULL)
			{
				tempElement->pPrev->pNext = tempElement->pNext;
			}
			else
			{
				root = tempElement->pNext;
			}

			free(tempElement);
			return true;
		}

		tempElement = tempElement->pNext;
	}

    return false;
}

PriorityQueue::Element_t *PriorityQueue::Find(int value)
{
	struct Element_t *tempElement = root;

	while (tempElement != NULL)
	{
		if (tempElement->value == value)
		{
			return tempElement;
		}

		tempElement = tempElement->pNext;
	}

    return NULL;
}

PriorityQueue::Element_t *PriorityQueue::GetHead()
{
    return root;
}

/*** Konec souboru tdd_code.cpp ***/
