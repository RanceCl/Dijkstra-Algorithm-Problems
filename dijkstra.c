/* C Programs from Chapter 10 of
 * Data Structures, Algorithms and Software Principles in C
 * by Thomas A. Standish
 */

/* Inputs:
 *     source:  the source vertex in the set V of vertices
 *     G:       a graph data structure that includes at least
 *         a.  count of number of vertices in the set V
 *         b.  either an adjacency matrix or adjacency lists
 *
 *           The code below assumes G contains a pointer to the matrix 
 *           T [ link_source ] [ link_destination ]
 *           but modifying the code to use adjacency lists is easy
 *
 * Outputs:
 *     ShortestDistance[]: an array for the path cost to each vertex from the 
 *                         source on the shortest path
 *     Predecessor[]:  an array for the predecessor vertex (or the upsteam
 *                     neighbor) on the shortest path to each vertex
 */
  
void ShortestPath(G, source)
{
    int count_added = 1;   // for verbose prints
    if (Verbose) {
        printf("Starting at node %d\n", source);
    }
    // W is a set indicating which vertices have been confirmed, that is,
    //   the vertices for which we have found the shortest distance

    // Initialize W and ShortestDistance[u] as follows:
    W = {source};
    ShortestDistance[source] = 0;
    for (each u in V - {source} ) {
        ShortestDistance[u] = T[source][u];
    }

    // repeatedly enlarge W until W includes all vertices in V 
    while (W != V) {

        // find the vertex w in V - W at the minimum distance from source
        MinDistance = 'infinity'; 
        w = 'invalid vertex';
        for (each v in V - W) {
            if (ShortestDistance[v] < MinDistance) {
                MinDistance = ShortestDistance[v];
                w = v;
            }
        }

        // if a vertex w was found, add w to W, the confirmed set
        W = W union {w};
        if (Verbose) {
            count_added++;
            printf("%d: %d confirmed with cost %g and predecessor %d\n", 
                    count_added, w, MinDistance, Predecessor[w]);
        }

        // update the shortest distances to vertices in V - W via w 
        for (each u in V - W) {
            // if vertex u is a neighbor of w
            if (T[w][u] < 'infinity') {
                cost_to_u_via_w = ShortestDistance[w] + T[w][u];
                if (cost_to_u_via_w < ShortestDistance[u]) {
                    ShortestDistance[u] = cost_to_u_via_w;
                    if (Verbose) {
                        printf("\tdest %d has lower cost %g with predecessor %d\n", u, cost_to_u_via_w, w);
                    }
                }
            }
        }
    }
    if (Verbose) {
        printf("Found %d nodes, including source\n", count_added);
    }
}
