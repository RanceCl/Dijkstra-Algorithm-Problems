/* graph.c 
 * Roderick "Rance" White
 * roderiw
 * Lab6: Graphs
 * ECE 2230, Fall 2020
 *
 * This file contains functions to be called by lab6.c to construct, manipulate, and
 * destroy graphs. 
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>
#include <float.h>

#include "graph.h"


/* Creates a graph G with NumVertices amount of vertices */
graph_t* graph_construct(int NumVertices)
{
	int i,j;

	graph_t *G;
    G = (graph_t *) malloc(sizeof(graph_t));
    G->EdgeCount = 0;
//    G->MaxCount = 0;
    G->NumVert = NumVertices;

	/* Allocate space for a 2D Adjacent Matrix */
    G->AdjMatrix = (double **) malloc(NumVertices * sizeof(double *));
	for(i=0; i<NumVertices; i++)
		G->AdjMatrix[i] = (double *) malloc(NumVertices * sizeof(double));

	/* Set each adjacency to Infinity and same nodes to 0, since no connections exist */
	for(i=0; i<NumVertices; i++){
		for(j=0; j<NumVertices; j++){
			if(i == j) G->AdjMatrix[i][j] = 0;		//Weight is 0 for node to itself 
			else G->AdjMatrix[i][j] = FLT_MAX;		//Weight is infinite if no edge
		}
	}

	/* Allocate space for a Vertex Array */
	G->VertArray = (GraphItem **) malloc(NumVertices * sizeof(GraphItem *));

	//Set all elements in the Vertex Array to NULL
	for(i=0; i<NumVertices; i++)
		G->VertArray[i] = NULL;

    return G;
}

/* Frees graph G */
void graph_destruct(graph_t *G)
{
	int i;

	/* Free matrix before freeing the rest of the structure */
	for(i=0; i<G->NumVert; i++)
		free(G->AdjMatrix[i]);
    free(G->AdjMatrix);

	/* Free elements in Vertex Array if it isn't empty */
	if(G->VertArray != NULL)
	{
		for(i=0; i<G->NumVert; i++)
		{
			free(G->VertArray[i]);
			G->VertArray[i] = NULL;
		}
	}

    free(G->VertArray);
    free(G);

}

/* Adds an edge from vertex LSource to LDest with a weight of LWeight */
void graph_add_edge(graph_t *G, int LSource, int LDest, double LWeight)
{
	G->EdgeCount = G->EdgeCount + 1;
	G->AdjMatrix[LSource][LDest] = LWeight;
}


/* Generates the node for LSource with the x, y coordinates */
GraphItem* graph_generate_node(int LSource, int NumVertices)
{
	GraphItem *Item;
	Item = (GraphItem *) malloc(sizeof(GraphItem));
	Item->node = LSource;

	/* Location of smallest and largest node are different */
	if(LSource == 0)
	{
		Item->xl=0.25;
		Item->yl=0.0;
	}
	else if(LSource == NumVertices-1)
	{
		Item->xl=0.75;
		Item->yl=1.0;
	}
	else
	{
		Item->xl=drand48();
		Item->yl=drand48();
	}

	Item->adjvert = 0;
	return Item;
}

/* Calculates the distance between vertex i and vertex j */
double vertex_distance(GraphItem* i, GraphItem* j)
{
	double a = i->xl - j->xl;
	double b = i->yl - j->yl;

	a = a * a;
	b = b * b;

	return sqrt(a + b);
}


double maximum_data(double D, double C)
{
	double F;

	F = D + (C/100);
	F = 1/F;
	F = 1+(F * F);
	F = log10(F);

	return F;
}

/* Calculates the weight between vertex LSource and vertex LDest */
double weight(int LSource, int LDest)
{
	double W, W1, W2, W3;

	//Weight is 0 if the values are the same
	if(LSource == LDest) return 0;

	//First portion of weight is an absolute value
	W1 = (LSource - LDest)/1.5;
	if(W1 < 0) W1 = W1 * -1;			//If negative, make positive

	//Second portion must be squared
	W2 = (LSource - LDest) + 1.5;
	W2 = W2 * W2;						//Square portion 2 of weight

	W3 = 1.5 * LDest;

	//Entire weight is the sum of the three parts
	W = W1 + W2 + W3;

	return W;
}

double FindMaxElement(double ElementArray[], int ElementCount, int *LDest, int *IsInfinity)
{
	int i;
	double ElemMax = -1;

	//Loop through all elements to find max
	for(i = 0; i < ElementCount; i++)
	{
		if(ElementArray[i] == FLT_MAX) *IsInfinity = 1;	//Check if no path exists for a pair
		else if(ElemMax < ElementArray[i])
		{
			*LDest = i;
			ElemMax = ElementArray[i]; 					//Update max
		}
	}
	return ElemMax;
}

/* A function to find the shortest path using Dijkstra's pseudocode */
void ShortestPath(graph_t *G, int LSource, int Verbose, int **Pred, double **ShDis)
{
	int u, v, w, VComp=0;
	double MinDistance, cost_to_u_via_w;
	int *V, *W, *Predecessor;
	double *ShortestDistance;

	/* Since arrays can't be of variable size, allocate space for arrays */
	V = (int *) malloc(G->NumVert * sizeof(int));
	W = (int *) malloc(G->NumVert * sizeof(int));
	Predecessor = (int *) malloc(G->NumVert * sizeof(int));
	ShortestDistance = (double *) malloc(G->NumVert * sizeof(double));

	int count_added = 1;	//For verbose prints
	if (Verbose) printf("Starting at node %d\n", LSource);

	// Initialize W and ShortestDistance[u] as follows:
	/* Initialize arrays and set empty spaces to 0's to avoid errors */
	for(u=0; u < G->NumVert; u++) {
		V[u] = u;						//Set Vertex array for later code
		W[u] = -1;
		Predecessor[u] = LSource;
		ShortestDistance[u] = G->AdjMatrix[LSource][u];
	}

	// Setting starting location
	W[LSource] = LSource;
	ShortestDistance[LSource] = 0;
	Predecessor[LSource] = LSource;

	//Loop to see if V and C are the same
	for(u=0; u < G->NumVert; u++) {
		if(V[u] != W[u]) VComp = 1;		//Must compare vertices of the matrix one at a time
	}

	/* Repeatedly enlarge W until W includes all vertices in V */
	while(VComp == 1)
	{
        // find the vertex w in V - W at the minimum distance from source
		MinDistance = FLT_MAX;
		w = G->NumVert;					//Invalid vertex needed, so exceed by 1

        for (v=0; v < G->NumVert; v++) 
		{
			if (W[v] == -1)				//Only for each v in V - W, so while W doesn't have vertex
			{
				//Distance can't be 0
		        if (ShortestDistance[v] < MinDistance) 
				{
		            MinDistance = ShortestDistance[v];
		            w = v;
		        }
			}
        }

        // if a vertex w was found, add w to W, the confirmed set
		if(w < G->NumVert)				//point is within the range
		{
		    W[w] = w;
		    if (Verbose) {
				count_added++;
		        printf("%d: %d confirmed with cost %g and predecessor %d\n", 
		                count_added, w, MinDistance, Predecessor[w]);
		    }
		    // update the shortest distances to vertices in V - W via w 
		    for (u=0; u < G->NumVert; u++) 
			{
				if (W[u] == -1)			//Only for each u in V - W, so while W doesn't have vertex
				{
		        	// if vertex u is a neighbor of w
					if (G->AdjMatrix[w][u] < FLT_MAX)
					{
		            	cost_to_u_via_w = ShortestDistance[w] + G->AdjMatrix[w][u];
		            	if (cost_to_u_via_w < ShortestDistance[u]) 
						{
		                	ShortestDistance[u] = cost_to_u_via_w;
							Predecessor[u]=w;
		                	if (Verbose) 
							{
		                    	printf("\tdest %d has lower cost %g with predecessor %d\n", u, cost_to_u_via_w, w);
		                	}
		            	}
		        	}
				}
		    }
		}
		else		//If the points can't be reached, those remaining are infinite
		{
			//Loop to find missing values in W
			for(u=0; u < G->NumVert; u++) {
				if(W[u] == -1)
				{
					W[u]=u;
					Predecessor[u] = u;
				}
			}
		}
		VComp = 0;

		//Loop to see if V and C are the same
		for(u=0; u < G->NumVert; u++) {
			if(V[u] != W[u]) VComp = 1;		//Must compare vertices of the matrix one at a time
		}
    }
    if (Verbose) printf("Found %d nodes, including source\n", count_added);

	*Pred = Predecessor;					//To return the predecessor matrix to the caller
	*ShDis = ShortestDistance;				//To return the shortest distance matrix to the caller

	free(V); free(W); 
}

/* Prints the shortest path and returns an array holding the backwards path */
int* PrintShortestPath(graph_t *G, int LSource, int LDest, int Pred[], double ShortDist[])
{
	int i=0, j=0, *ShortPath;

	/* Since arrays can't be of variable size, allocate space for arrays */
	//Add one more for an integer to act as an ending point
	ShortPath = (int *) malloc(G->NumVert * sizeof(int));
	i=LDest;

	/* Loop to put shortest path between points into its own array */
	while(i != LSource && ShortDist[i] != FLT_MAX)
	{
		ShortPath[j]=i;					//Path placed in array backwards
		j++;
		i=Pred[i];
	}

	//If cost is infinite, say that the path doesn't exist
	if(ShortDist[i] == FLT_MAX)
	{
		ShortPath[0]=LSource;
		printf("\nThere is no path between %d and %d.\n", LSource,LDest);
	}
	else 									//Otherwise, place beginning vertex and print path
	{
		ShortPath[j]=LSource;				//Place source at the end
		printf("\nThe cost of the path between %d and %d is %.2f\n",LSource,LDest,ShortDist[LDest]);

		for(j=j; j>0; j--) printf("%d -- ", ShortPath[j]);	
		printf("%d\n", ShortPath[j]);		//Printed separately to not include the "--"
	}

	return ShortPath;						//Only used for command 3
}








