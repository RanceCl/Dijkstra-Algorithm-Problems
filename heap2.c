/* heap2.c
 * ECE 2230 Fall 2020
 * Harlan Russell
 *
 * A version of a priority queue using a heap.
 *
 * The heap keeps a pointer to the location of an item in the heap so that if
 * an updated entry is inserted into the heap the heap location is updated
 * rather than inserting a new item.
 *
 * For graph applications this prevents duplicate entries for the same vertex.
 *
 * However, the memory usage increases because a list of all vertices must be
 * maintained.  
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "heap.h"

/*--------------------------------------------------*/

PriorityQueue *PQInitialize(int num_vertices)
{
    PriorityQueue *PQ;
    PQ = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    PQ->Count = 0;
    PQ->Size = 16;
    PQ->MaxCount = 0;
    PQ->NumVertices = num_vertices;
    PQ->ItemArray = (PQItem *) malloc(PQ->Size * sizeof(PQItem));
    // ItemLocation is a very sparce table if the graph is sparce
    // consider improving implementation to use a hash table
    PQ->ItemLocation = (int *) calloc(num_vertices, sizeof(int));
    return PQ;
}
void PQDestruct(PriorityQueue *PQ)
{
    free(PQ->ItemArray);
    free(PQ->ItemLocation);
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

    assert(Item.node >=0 && Item.node < PQ->NumVertices);
    assert(Item.weight > 0.0);

    if (PQFull(PQ)) {    // could improve to first check if found
        printf("Call to Full failed!\n");
        exit(1);
    }
    // If the Item is already in heap then update
    if (PQ->ItemLocation[Item.node] != 0) {
        ChildLoc = PQ->ItemLocation[Item.node];
    }
    else {
        (PQ->Count)++;
        ChildLoc = PQ->Count;
        if (PQ->Count > PQ->MaxCount) PQ->MaxCount = PQ->Count;
    }
    ParentLoc = ChildLoc/2; 

    while (ParentLoc != 0) {             /* while a parent still exists */
        if (Item.weight >= PQ->ItemArray[ParentLoc].weight) {
            PQ->ItemLocation[Item.node] = ChildLoc;
            PQ->ItemArray[ChildLoc] = Item;                 /* store Item */
            return;                                         /* and return */
        } else {                /* here, Item > PQ->ItemArray[ParentLoc] */
            PQ->ItemLocation[PQ->ItemArray[ParentLoc].node] = ChildLoc;
            PQ->ItemArray[ChildLoc] = PQ->ItemArray[ParentLoc];
            ChildLoc = ParentLoc;
            ParentLoc = ParentLoc/2;
        }
    }

    PQ->ItemLocation[Item.node] = ChildLoc;
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
            PQ->ItemLocation[ItemToPlace.node] = CurrentLoc;
            PQ->ItemArray[CurrentLoc] = ItemToPlace;
            PQ->ItemLocation[ItemToReturn.node] = 0;
            return (ItemToReturn);
        } else {
            PQ->ItemLocation[PQ->ItemArray[ChildLoc].node] = CurrentLoc;
            PQ->ItemArray[CurrentLoc]=PQ->ItemArray[ChildLoc];
            CurrentLoc = ChildLoc;
            ChildLoc = 2 * CurrentLoc;
        }
    }

    /* final placement of ItemToPlace */
    PQ->ItemLocation[ItemToPlace.node] = CurrentLoc;
    PQ->ItemArray[CurrentLoc] = ItemToPlace;

    /* return the Item originally at the root */
    PQ->ItemLocation[ItemToReturn.node] = 0;
    return (ItemToReturn);
}
