/* heap.h
 * ECE 2230 Fall 2020
 * Harlan Russell
 *
 * Based on implementation from Standish's textbook.
 *
 *   The Priority Queue Types Header File "PQTypes.h"
 */

typedef  struct {
    int node;
    double weight;
} PQItem;

typedef  struct {
    int      Size;
    int      Count;
    int      MaxCount;
    int      NumVertices;      // only used in heap2.c
    PQItem   *ItemArray;
    int      *ItemLocation;    // only used in heap2.c
} PriorityQueue;


extern PriorityQueue *PQInitialize(int);         /* sets PQ to be empty */
void PQDestruct(PriorityQueue *PQ);
extern int PQEmpty(PriorityQueue *PQ);               /* true if PQ is empty */
extern int PQFull(PriorityQueue *PQ);                 /* true if PQ is full */
extern void PQInsert(PQItem Item, PriorityQueue *PQ);  /* puts Item into PQ */
extern PQItem PQRemove(PriorityQueue *PQ);          /* removes Item from PQ */

