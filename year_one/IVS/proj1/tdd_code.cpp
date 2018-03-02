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
 * @author JMENO PRIJMENI
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
    m_pHead = NULL;
}

PriorityQueue::~PriorityQueue()
{
	Element_t *temp = m_pHead;
	Element_t *next = NULL;

	if (m_pHead!=NULL) {
		while (temp!=NULL) {
				next = temp->pNext;
				delete(temp);
				temp = next;
		}
	}
}

void PriorityQueue::Insert(int value)
{
    // Prazdna fronta
    if (m_pHead == NULL) {
        m_pHead = new Element_t;
        m_pHead->pNext = NULL;
        m_pHead->pPrev = NULL;
        m_pHead->value = value;
        return ;
    }
    // Neprazdna fronta
    if (value < m_pHead->value) {
        Element_t *newel = new Element_t;
        newel->value = value;
        newel->pPrev = NULL;
        newel->pNext = m_pHead;
        m_pHead->pPrev = newel;
        m_pHead = newel;
        return ;
    }
    else {
        Element_t *temp = m_pHead;
        while (temp->pNext != NULL && temp->pNext->value<value) {
            temp = temp->pNext;
        }
        //Naviazanie noveho prvku
        Element_t *elem = new Element_t;
        elem->pNext = temp->pNext;
        elem->pPrev = temp;
        elem->value = value;
        //Naviazanie ostatnych prvkov
        //Koniec zoznamu alebo Druhy prvok
        if (temp->pNext != NULL) {
            temp->pNext->pPrev = elem;
        }
        temp->pNext = elem;
    }
}

bool PriorityQueue::Remove(int value)
{
    //Prazdny zoznam
    if (m_pHead==NULL)
        return false;
    //Neprazdny zoznam
    //Prvy prvok
    if (m_pHead->pNext==NULL) {
        if (m_pHead->value == value) {
            delete(m_pHead);
            m_pHead=NULL;
            return true;
        }
        else
            return false;
    }

    Element_t *temp = m_pHead;
    while (temp->pNext!=NULL && temp->value!=value) {
        temp = temp->pNext;
    }
    //Posledny prvok
    if (temp->pNext==NULL) {
        if (temp->value!=value) {
            return false;
        }
        else {
            temp->pPrev->pNext = NULL;
            delete(temp);
            return true;
        }
    }
    //Prvy prvok
    if (temp->pPrev==NULL) {
        m_pHead = m_pHead->pNext;
        m_pHead->pPrev = NULL;
        delete(temp);
        return true;
    }
    Element_t *rmv = temp;
    rmv->pPrev->pNext = rmv->pNext;
    rmv->pNext->pPrev = rmv->pPrev;
    delete(rmv);
    return true;
}

PriorityQueue::Element_t *PriorityQueue::Find(int value)
{
    if (m_pHead == NULL)
        return NULL;
    Element_t *temp = m_pHead;
    while (temp!=NULL && temp->value!=value)
        temp = temp->pNext;
    return temp;
}

PriorityQueue::Element_t *PriorityQueue::GetHead()
{
    return m_pHead;
}

/*** Konec souboru tdd_code.cpp ***/
