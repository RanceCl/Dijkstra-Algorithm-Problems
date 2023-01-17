/* lab6.c 
 * Roderick "Rance" White
 * roderiw
 * Lab6: Graphs
 * ECE 2230, Fall 2020
 *
 * This file contains drivers to form graphs and perform various operations on them
 *
 * Graph types
 *	 -g 1|2|3|4		type of graph (default 1)
 *	 -g 1			Weakly-connected directed graph
 *	 -g 2			Directed graph with symmetric weights
 *	 -g 3			Strongly-connected directed graph
 *	 -g 4			Random graph
 *
 *	 -n N			number of vertices in graph
 *	 -a 0<R<N		number of vertices in graph
 * 
 *	 -h 1|2|3		graph operation (default 1)
 *	 -h 1			Shortest path from vertex S to vertex D
 *	 -h 2			Network diameter of graph
 *	 -h 3			Multiple link-disjoint paths from vertex S to vertex D
 * 
 *	 -s 0<=S<N		number of the source vertex
 *	 -d 0<=D<N		number of the destination vertex
 *	 -v				turn on verbose prints (default off)
 *	 -r 1234567		seed for random number generator
 *	 -p				turns on debug print (added for personal use)
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

/* Global variables for command line parameters */
int GraphType = 1;
int NumberOfVertices = 20;
int AdjacentVertices = 5;
int GraphOperation = 0;
int NumberOfSourceVertex = 0;
int NumberOfDestinationVertex = 0;
int Verbose = FALSE;
int Printer = FALSE;
int Seed = 10212018;

/* Local functions */
void getCommandLine(int argc, char **argv);
void debug_print_graph(graph_t *G);
void AdjacencyPrint(graph_t *G);

void WeakConnectFunc(graph_t *G);
void DirectSymFunc(graph_t *G);
void StrongConnectFunc(graph_t *G);
void RandGraphFunc(graph_t *G);

void ShortestPathCommand(graph_t *G);
void NetworkDiameterCommand(graph_t *G);
void MultLinkCommand(graph_t *G);


int main(int argc, char **argv)
{
	graph_t *G;

	getCommandLine(argc, argv);
	printf("Seed: %d\n", Seed);
	srand48(Seed);

	G=graph_construct(NumberOfVertices);

	switch(GraphType) {
		case 1: 
			WeakConnectFunc(G);
			printf("\nWeakly-connected directed graph\n");
			break;
		case 2: 
			DirectSymFunc(G);
			printf("\nDirected graph with symmetric weights\n");
			break;
		case 3: 
			StrongConnectFunc(G);
			printf("\nStrongly-connected directed graph with %d vertices\n", NumberOfVertices);
			break;
		case 4: 
			RandGraphFunc(G);
			break;
		default: 
			fprintf(stderr, "invalid graph type: %d\n", GraphType);
			exit(1);
		}

	/* Command switch case */
	switch (GraphOperation) {
		/* ShortPath Command */
		case 1:
			printf("\nShort Path\n");
			ShortestPathCommand(G);
			break;

		/* Network Diameter Command */
		case 2:
			printf("\nNetwork Diameter\n");
			NetworkDiameterCommand(G);
			break;

		/* Multiple Link-disjoint Paths command */
		case 3:
			printf("\nMultiple link-disjoint paths\n");
			MultLinkCommand(G);
			break;
	}

	if(Printer)
		debug_print_graph(G);

	graph_destruct(G);		//Must destroy graph when done
	return 0;
}






/* --------------------FUNCTIONS FOR GRAPH CREATION-------------------- */

/* Function for setting up weakly-connected directed graph */
void WeakConnectFunc(graph_t *G)
{
	assert(NumberOfVertices == 7);
	graph_add_edge(G, 1, 2, 3);
	graph_add_edge(G, 1, 6, 5);
	graph_add_edge(G, 2, 3, 7);
	graph_add_edge(G, 2, 4, 3);
	graph_add_edge(G, 2, 6, 10);
	graph_add_edge(G, 3, 4, 5);
	graph_add_edge(G, 3, 5, 1);
	graph_add_edge(G, 4, 5, 6);
	graph_add_edge(G, 5, 6, 7);
	graph_add_edge(G, 6, 0, 4);
	graph_add_edge(G, 6, 2, 5);
	graph_add_edge(G, 6, 3, 8);
	graph_add_edge(G, 6, 4, 9);
}

/* Function for setting up directed graph with symmetric weights */
void DirectSymFunc(graph_t *G)
{
	assert(NumberOfVertices == 10);
	graph_add_edge(G, 0, 1, 2);
	graph_add_edge(G, 0, 2, 1);
	graph_add_edge(G, 0, 3, 6);
	graph_add_edge(G, 1, 0, 2);
	graph_add_edge(G, 1, 4, 1);
	graph_add_edge(G, 1, 8, 3);
	graph_add_edge(G, 2, 0, 1);
	graph_add_edge(G, 2, 4, 3);
	graph_add_edge(G, 2, 5, 7);
	graph_add_edge(G, 3, 0, 6);
	graph_add_edge(G, 3, 5, 1);
	graph_add_edge(G, 3, 9, 5);
	graph_add_edge(G, 4, 1, 1);
	graph_add_edge(G, 4, 2, 3);
	graph_add_edge(G, 4, 6, 4);
	graph_add_edge(G, 5, 2, 7);
	graph_add_edge(G, 5, 3, 1);
	graph_add_edge(G, 5, 7, 4);
	graph_add_edge(G, 6, 4, 4);
	graph_add_edge(G, 6, 7, 5);
	graph_add_edge(G, 6, 8, 1);
	graph_add_edge(G, 7, 5, 4);
	graph_add_edge(G, 7, 6, 5);
	graph_add_edge(G, 7, 9, 1);
	graph_add_edge(G, 8, 1, 3);
	graph_add_edge(G, 8, 6, 1);
	graph_add_edge(G, 8, 9, 4);
	graph_add_edge(G, 9, 3, 5);
	graph_add_edge(G, 9, 7, 1);
	graph_add_edge(G, 9, 8, 4);
}


/* Function for setting up strongly-connected directed graph */
void StrongConnectFunc(graph_t *G)
{
	int link_src, link_dest;

	/* Loop to add appropriate edges for each vertex */
	for(link_src = 0; link_src < NumberOfVertices; link_src++)
	{ 
		for(link_dest = 0; link_dest < NumberOfVertices; link_dest++) 
		{
			if(link_src == link_dest) continue;
			graph_add_edge(G, link_src, link_dest, weight(link_src, link_dest));
		}
	}
}


/* Function for setting up random graph */
void RandGraphFunc(graph_t *G)
{
	int i, link_src, link_dest, TempMax, TempMin, TempAvg;
	double distance, VertRad, MinDatRate, MaxDatRate;

	GraphItem *NewItem;

	/* Creating the node locations */
	for(i = 0; i < NumberOfVertices; i++)
	{
		// Generate the new node
		NewItem = graph_generate_node(i, NumberOfVertices);
		G->VertArray[i]=NewItem;				//Place new node in proper spot
	}

	//Find radius of circle for later
	VertRad=sqrt(AdjacentVertices/(3.14 * NumberOfVertices));

	//Find minimum data rate for later
	MinDatRate=log10(1+((1/VertRad)*(1/VertRad)));

	/* Loop to add appropriate edges for each vertex */
	for(link_src = 0; link_src < NumberOfVertices; link_src++)
	{ 
		for(link_dest = 0; link_dest < NumberOfVertices; link_dest++) 
		{
			if(link_src == link_dest) continue;

			/* Functions for finding weight */
			//Find distance between points first
			distance=vertex_distance(G->VertArray[link_src], G->VertArray[link_dest]);

			//Weight for edge depends on following
			if(distance <= VertRad)
			{
				//Find maximum data rate
				MaxDatRate=maximum_data(distance, VertRad);
				graph_add_edge(G, link_src, link_dest, MinDatRate/MaxDatRate);

				/* Count up adjacent edges */
				G->VertArray[link_src]->adjvert=G->VertArray[link_src]->adjvert+1;
			}
			else graph_add_edge(G, link_src, link_dest, FLT_MAX);
		}
	}
	printf("\nRandom graph with %d vertices\n", NumberOfVertices);

	TempMax = G->VertArray[0]->adjvert;
	TempMin = G->VertArray[0]->adjvert;
	TempAvg = G->VertArray[0]->adjvert;

	/* Find minimum, maximum, and average adjacent vertices */
	for(i = 1; i < NumberOfVertices; i++)	
	{
		//Update max if the value is larger
		if(TempMax < G->VertArray[i]->adjvert) TempMax = G->VertArray[i]->adjvert;
		//Update min if the value is smaller
		if(TempMin > G->VertArray[i]->adjvert) TempMin = G->VertArray[i]->adjvert;
		TempAvg = TempAvg + G->VertArray[i]->adjvert;
	}
	TempAvg = TempAvg/NumberOfVertices;

	printf("Minimum adjacent vertices: %d\n", TempMin);
	printf("Maximum adjacent vertices: %d\n", TempMax);
	printf("Average adjacent vertices: %d\n", TempAvg);

}





/* --------------------FUNCTIONS FOR GRAPH OPERATIONS-------------------- */

/* Function for the Shortest Path Command */
void ShortestPathCommand(graph_t *G)
{
	int *Pred, *TempPred;
	double *ShortDist;
	clock_t start, end;

	/* Timer for the Dijkstra function */
	start = clock();
	ShortestPath(G, NumberOfSourceVertex, Verbose, &Pred, &ShortDist);
	end = clock();

	TempPred=PrintShortestPath(G, NumberOfSourceVertex, NumberOfDestinationVertex, Pred, ShortDist);

	free(Pred); free(ShortDist); free(TempPred);	//Free allocated arrays

	printf("After ShortestPath command on a graph with %d vertices,", NumberOfVertices);
	printf("\ntime = %g ms\n\n", 1000*((double)(end-start))/CLOCKS_PER_SEC);
}


/* Function for the Network Diameter Command */
void NetworkDiameterCommand(graph_t *G)
{
	int i, j=-1, LSource=-2, LDest=-1, IsInfinity=0, *Pred, *TempPred;
	double MaxElem, TempMax, *ShortDist, *TempShort;
	clock_t start, end;

	MaxElem =-1;
	ShortestPath(G, 0, Verbose, &Pred, &ShortDist);		//First case

	/* Timer to measure how long the actual command takes */
	start = clock();
	for(i=0; i < G->NumVert; i++)
	{
		ShortestPath(G, i, Verbose, &TempPred, &TempShort);
		TempMax = FindMaxElement(TempShort, NumberOfVertices, &j, &IsInfinity);

		//Update max value, source, dest, and paths if a larger max is found
		if(MaxElem < TempMax)
		{
			MaxElem = TempMax;
			free(Pred); free(ShortDist);		//Must be freed before being replaced
			ShortestPath(G, i, Verbose, &Pred, &ShortDist);
			LSource = i;
			LDest = j;
		}
		free(TempPred); free(TempShort);
	}
	end = clock();

	TempPred = PrintShortestPath(G, LSource, LDest, Pred, ShortDist);
	free(Pred);free(ShortDist);free(TempPred);

	if(IsInfinity > 0) printf("There is at least one path without a connection.\n");

	printf("After NetworkDiameter command on a graph with %d vertices,", NumberOfVertices);
	printf("\ntime = %g ms\n\n", 1000*((double)(end-start))/CLOCKS_PER_SEC);
}





/* Function for the Multiple Link-Disjoint Command */
void MultLinkCommand(graph_t *G)
{
	int n=0, *Pred, *TempPred;
	double *ShortDist;
	clock_t start, end;

	/* Timer to measure how long the actual command takes */
	start = clock();
	/* Initial path find */
	ShortestPath(G, NumberOfSourceVertex, Verbose, &Pred, &ShortDist);
	TempPred=PrintShortestPath(G, NumberOfSourceVertex, NumberOfDestinationVertex, Pred, ShortDist);

	//Stop looping when there are no more paths to the destination
	while(TempPred[0] != NumberOfSourceVertex && TempPred[0] != FLT_MAX)
	{
		n = 0;				//Reset n to 0 for the removing loop
		/* Loop to remove edges on the previous path */
		while(TempPred[n] != NumberOfSourceVertex && TempPred[n] != FLT_MAX)
		{
			G->AdjMatrix[TempPred[n+1]][TempPred[n]]=FLT_MAX;	//Set path to infinity
			n++;
		}

		free(Pred); free(ShortDist); free(TempPred);	//Free allocated arrays before using again

		//Get new path
		ShortestPath(G, NumberOfSourceVertex, Verbose, &Pred, &ShortDist);
		TempPred=PrintShortestPath(G,NumberOfSourceVertex,NumberOfDestinationVertex,Pred,ShortDist);
	}

	end = clock();
	printf("After Multiple link-disjoint command with %d vertices,", NumberOfVertices);
	printf("\ntime = %g ms\n\n", 1000*((double)(end-start))/CLOCKS_PER_SEC);
	free(Pred); free(ShortDist); free(TempPred);		//Final free of allocated arrays

}







/* --------------------PRINTING FUNCTION-------------------- */


void debug_print_graph(graph_t *G)
{
	int i;

	assert(G->NumVert == NumberOfVertices);

	/* Print aspects of graph */

	switch(GraphType) {
		case 1: 
			assert(G->NumVert == 7);		//Graph type 1 will only have 7 vertices
			assert(G->EdgeCount == 13);		//Graph type 1 will only have 13 edges
			AdjacencyPrint(G);
			printf("\nWeakly-connected directed graph\n");
			break;
		case 2: 
			assert(G->NumVert == 10);		//Graph type 2 will only have 10 vertices
			assert(G->EdgeCount == 30);		//Graph type 2 will only have 30 edges
			AdjacencyPrint(G);
			printf("\nDirected graph with symmetric weights\n");
			break;
		case 3:
			assert(G->EdgeCount == (G->NumVert * (G->NumVert - 1)));
			AdjacencyPrint(G);
			printf("\nStrongly-connected directed graph\n");
			break;
		case 4:
			AdjacencyPrint(G);
			/* Printing the graph of nodes if graph type 4 */
			printf("\nNode List\n");
			for(i=0; i<G->NumVert; i++)
			{
				printf("Vertex %d: (%.2f, %.2f) %d connections\n",G->VertArray[i]->node,G->VertArray[i]->xl,G->VertArray[i]->yl,G->VertArray[i]->adjvert);
			}
			printf("\nRandom graph\n");
			break;
	}

	//Print aspects of graph
	printf("%d vertices and %d edges:\n",G->NumVert,G->EdgeCount);
}

/* Prints the graphs adjacency matrix */
void AdjacencyPrint(graph_t *G)
{
	int i, j;

	/* Printing of adjacency matrix */
	printf("\nAdjacency Matrix");
	printf("\n%c", 'X');
	for(i=0; i<G->NumVert; i++) printf(" %4d", i);

	printf("\n");

	for(i=0; i<G->NumVert; i++)
	{
		printf("%d", i);
		for(j=0; j<G->NumVert; j++){
			//Print INFIN if the weight is infinite
			if(G->AdjMatrix[i][j] == FLT_MAX)
				printf(" %4s", "INF");
			else
				printf(" %4.2f", G->AdjMatrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}



/* --------------------COMMAND LINE READ FUNCTION-------------------- */

/* read in command line arguments and store in global variables for easy
 * access by other functions.
 */
void getCommandLine(int argc, char **argv)
{
	/* optopt--if an unknown option character is found
	 * optind--index of next element in argv 
	 * optarg--argument for option that requires argument 
	 * "x:" colon after x means argument required
	 */
	int c;
	int index;

	while ((c = getopt(argc, argv, "g:n:a:h:s:d:r:vp")) != -1)
		switch(c) {
			case 'g': 
				if(1 <= atoi(optarg) && atoi(optarg) <= 4) {
					GraphType = atoi(optarg);
					if(GraphType == 1) NumberOfVertices = 7;
					if(GraphType == 2) NumberOfVertices = 10;
				}
				else {
					fprintf(stderr, "invalid graph type: %s\n", optarg);
					exit(1);
				}
				break;
			case 'n': NumberOfVertices = atoi(optarg);			break;
			case 'a': AdjacentVertices = atoi(optarg);			break;
			case 'h': 
				if(1 <= atoi(optarg) && atoi(optarg) <= 3)
					GraphOperation = atoi(optarg);
				else {
					fprintf(stderr, "invalid graph operation: %s\n", optarg);
					exit(1);
				}
				break;
			case 's': NumberOfSourceVertex = atoi(optarg);		break;
			case 'd': NumberOfDestinationVertex = atoi(optarg);	break;
			case 'r': Seed = atoi(optarg);						break;
			case 'v': Verbose = TRUE;							break;
			case 'p': Printer = TRUE;							break;
			case '?':
				if (isprint(optopt))
					fprintf(stderr, "Unknown option %c.\n", optopt);
				else
					fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
			default:
				printf("Lab6 command line options\n");
				printf("General options ---------\n");
				printf("  -g 1|2|3|4    type of graph (default 1)\n");
				printf("  -n N          number of vertices in graph\n");
				printf("  -a 0<R<N      number of vertices in graph\n");
				printf("  -h 1|2|3      graph operation (default 1)\n");
				printf("  -s 0<=S<N     number of the source vertex\n");
				printf("  -d 0<=D<N     number of the destination vertex\n");
				printf("  -v            turn on verbose prints (default off)\n");
				printf("  -p            turn on debug print (default off) (NOT recommended for large graphs\n");
				printf("  -r 1234567    seed for random number generator\n");
				exit(1);
		}
	for (index = optind; index < argc; index++)
		printf("Non-option argument %s\n", argv[index]);



	/* Make sure all values are viable */
	if(!(0 < AdjacentVertices && AdjacentVertices < NumberOfVertices))
	{
		fprintf(stderr, "invalid number of adjacent vertices : %d\n", AdjacentVertices);
		exit(1);
	}

	if(!(0 <= NumberOfSourceVertex && NumberOfSourceVertex < NumberOfVertices))
	{
		fprintf(stderr, "invalid number of source vertex: %d\n", NumberOfSourceVertex);
		exit(1);
	}

	if(!(0 <= NumberOfDestinationVertex && NumberOfDestinationVertex < NumberOfVertices))
	{
		fprintf(stderr, "invalid number of destination vertex: %d\n", NumberOfSourceVertex);
		exit(1);
	}

}

