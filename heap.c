/* heap.c
 * ECE 2230 Fall 2020
 * Harlan Russell
 *
 * A version of a priority queue using a heap.  This code is the same
 * as the code from Standish except PQFull reallocs array and the 
 * highest priority has the smallest instead of largest value.
 *
 * This heap does not check if the Item that is inserted is already 
 * in the heap.  See heap2.c for a version that checks for duplicates.
 */
/*  ------------< begin file "PQImplementation.c" >------------  */

#include <stdlib.h>
#include <stdio.h>
#include "heap.h"

/*--------------------------------------------------*/

/* note num_vertices is not used but is needed so interface for heap and heap2
 * are the same.  
 */
PriorityQueue *PQInitialize(int num_vertices)
{
    PriorityQueue *PQ;
    PQ = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    PQ->Count = 0;
    PQ->MaxCount = 0;
    PQ->Size = 16;
    PQ->ItemArray = (PQItem *) malloc(PQ->Size * sizeof(PQItem));
    return PQ;
}
void PQDestruct(PriorityQueue *PQ)
{
    free(PQ->ItemArray);
    free(PQ);
}


/*--------------------------------------------------*/
int PQEmpty(PriorityQueue *PQ)
{
    // realloc to reduce size if small
    return (PQ->Count == 0);
}
/*--------------------------------------------------*/

/* PQ is full when the count is size - 1, since the PQ
 * starts at position 1 not 0.
 */
int PQFull(PriorityQueue *PQ)
{
    if (PQ->Count == PQ->Size - 1)
    {
        PQ->Size *= 2;
        PQ->ItemArray = (PQItem *) realloc(PQ->ItemArray, PQ->Size * sizeof(PQItem));
        if (PQ->ItemArray == NULL)
        {
            printf("Failed to double priority queue!\n");
            exit(1);
        }
    }

    return (PQ->Count == PQ->Size - 1);
}

/*--------------------------------------------------*/

void PQInsert(PQItem Item, PriorityQueue *PQ)
{   
    int  ChildLoc;                         /* location of current child */
    int  ParentLoc;                          /* parent of current child */

    if (PQFull(PQ)) {
        printf("Call to Full failed!\n");
        exit(1);
    }
    (PQ->Count)++;
    if (PQ->Count > PQ->MaxCount) PQ->MaxCount = PQ->Count;
    ChildLoc = PQ->Count;
    ParentLoc = ChildLoc/2; 

    while (ParentLoc != 0) {             /* while a parent still exists */
        if (Item.weight >= PQ->ItemArray[ParentLoc].weight) {
            PQ->ItemArray[ChildLoc] = Item;                 /* store Item */
            return;                                         /* and return */
        } else {                /* here, Item > PQ->ItemArray[ParentLoc] */
            PQ->ItemArray[ChildLoc] = PQ->ItemArray[ParentLoc];
            ChildLoc = ParentLoc;
            ParentLoc = ParentLoc/2;
        }
    }

    PQ->ItemArray[ChildLoc] = Item;  /* Put Item in final resting place */

}

/*--------------------------------------------------*/

PQItem PQRemove(PriorityQueue *PQ)
{
    int     CurrentLoc;            /* location currently being examined */
    int     ChildLoc;                          /* a child of CurrentLoc */
    PQItem  ItemToPlace;                   /* an Item value to relocate */
    PQItem  ItemToReturn;           /* the removed Item value to return */


    if (PQEmpty(PQ))        /* result is undefined if PQ was empty */
    {
        printf("Attempting to remove from empty pq\n");
        exit(1);
    }

    /* Initializations */
    ItemToReturn = PQ->ItemArray[1];        /* value to return later */
    ItemToPlace = PQ->ItemArray[PQ->Count];     /* last leaf's value */
    (PQ->Count)--;                /* delete last leaf in level order */
    CurrentLoc = 1;                     /* CurrentLoc starts at root */
    ChildLoc = 2*CurrentLoc; /* ChildLoc starts at root's left child */


    while (ChildLoc <= PQ->Count) {       /* while a child still exists */

        /* Set ChildLoc to location of larger child of CurrentLoc */
        if (ChildLoc < PQ->Count) {          /* if right child exists */
            if  ( PQ->ItemArray[ChildLoc+1].weight < 
                    PQ->ItemArray[ChildLoc].weight) {
                ChildLoc++;
            }
        }
        /* If item at ChildLoc is larger than ItemToPlace, */
        /* move this larger item to CurrentLoc, and move */
        /* CurrentLoc down. */
        if (PQ->ItemArray[ChildLoc].weight >= ItemToPlace.weight) {
            PQ->ItemArray[CurrentLoc] = ItemToPlace;
            return (ItemToReturn);
        } else {
            PQ->ItemArray[CurrentLoc]=PQ->ItemArray[ChildLoc];
            CurrentLoc = ChildLoc;
            ChildLoc = 2 * CurrentLoc;
        }
    }

    /* final placement of ItemToPlace */
    PQ->ItemArray[CurrentLoc] = ItemToPlace;

    /* return the Item originally at the root */
    return (ItemToReturn);
}
