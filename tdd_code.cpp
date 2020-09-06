//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - priority queue code
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     DAVID MIHOLA <xmihol00@stud.fit.vutbr.cz>
// $Date:       $2020-20-02
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author DAVID MIHOLA
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
    head = NULL;
}

PriorityQueue::~PriorityQueue()
{
    Element_t *tmp;
    while (head)
    {
        tmp = head->pNext;
        delete head;
        head = tmp;
    }
}

void PriorityQueue::Insert(const int value)
{
    Element_t *added_node = new Element_t;
    if (added_node == NULL) //<iostream> not included for c++ exception handling
    {
        fprintf(stderr, "Error occured while allocation new memory.\n");
        fprintf(stderr, "Insertion unsuccessful.\n");
        return;
    }
    added_node->value = value;

    Element_t *tmp = head;

    if (head == NULL)
    {
        head = added_node;
        head->pPrev = NULL;
        head->pNext = NULL;
        return;
    }

    while (tmp)
    {
        if (tmp->value >= value)
        {
            if (tmp->pPrev == NULL)
            {
                tmp->pPrev = added_node;
                added_node->pPrev = NULL;
                added_node->pNext = tmp;
                head = added_node;
                return;
            }
            else
            {
                added_node->pPrev = tmp->pPrev;
                tmp->pPrev->pNext = added_node;
                added_node->pNext = tmp;
                tmp->pPrev = added_node;
                return;
            }
        }
        else if (tmp->pNext == NULL)
        {
            tmp->pNext = added_node;
            added_node->pPrev = tmp;
            added_node->pNext = NULL;
            return;
        }

        tmp = tmp->pNext;
    }
}

bool PriorityQueue::Remove(const int value)
{
    Element_t *tmp = head;
    while (tmp)
    {
        if (tmp->value == value)
        {
            if (tmp->pNext)
                tmp->pNext->pPrev = tmp->pPrev;
            if (tmp->pPrev)
                tmp->pPrev->pNext = tmp->pNext;
            else
                head = tmp->pNext;

            delete tmp;
            return true;
        }
        tmp = tmp->pNext;
    }
    return false;
}

PriorityQueue::Element_t *PriorityQueue::Find(const int value)
{
    Element_t *tmp = head;
    while (tmp)
    {
        if (tmp->value == value)
            return tmp;

        tmp = tmp->pNext;
    }

    return NULL;
}

PriorityQueue::Element_t *PriorityQueue::GetHead()
{
    return head;
}

/*** Konec souboru tdd_code.cpp ***/
