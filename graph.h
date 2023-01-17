/* graph.h
 * Roderick "Rance" White
 * roderiw
 * Lab6: Graphs
 * ECE 2230, Fall 2020
 *
 * Functions and structures for the formation of the graphs.
 */


#define TRUE 1
#define FALSE 0

typedef  struct {
    int node;
	double xl;
	double yl;
	int adjvert;
} GraphItem;

typedef  struct {
    int      	EdgeCount;
    int      	NumVert;
    double   	**AdjMatrix;
    GraphItem   **VertArray;
} graph_t;




graph_t* graph_construct(int NumVertices);
void graph_destruct(graph_t *G);
void graph_add_edge(graph_t *G, int LSource, int LDest, double LWeight);

GraphItem* graph_generate_node(int LSource, int NumVertices);

double vertex_distance(GraphItem* i, GraphItem* j);
double maximum_data(double D, double C);
double weight(int LSource, int LDest);
double FindMaxElement(double ElementArray[], int ElementCount, int *LDest, int *IsInfinity);

void ShortestPath(graph_t *G, int LSource, int Verbose, int **Pred, double **ShDis);

int* PrintShortestPath(graph_t *G, int LSource, int LDest, int Pred[], double ShortDist[]);

