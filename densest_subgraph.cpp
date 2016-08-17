/**
 * Compute the densest subgraph given a graph in edge-list format.
 *
 * The edge list format needs to be symmetric, so if (ei[k1],ej[k1]) = (r,s) then there must be (ei[k2],ej[k2]) = (s,r),
 * unless r==s.
 * 
 * @param [in] n the number of nodes of the graph
 * @param [in] m the number of edges of the graph (and also the length of arrays ei, ej, w)
 * @param [in] ei a list of sources for each edge (0 <= ei[i] <= n-1) of length m
 * @param [in] ej a list of destinations for each edge (0 <= ei[i] <= n-1) of length m
 * @param [in] w a list of non-negative weights for each edge (0 <= w[i]) of length m
 * @param [out] output a list of vertices in the densest subgraph used, 0 <= output[i] <= n-1 for 0 <= i <= outputlen-1, this array must be capable of holding n
 * @param [in/out] the valid length of the output list and the length of the set of output used. 
 * @return the density of the densest subgraph in total_edge_weight/number of vertices
 */

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <numeric>
#include "def.h"

using namespace std;

int nverts, nedges, src, dest;
double g;

void init_edges(double (*edges_info)[3], double *degree, int64_t n, int64_t m);
void free_malloc();


double densest_subgraph(int64_t n, int64_t m, int64_t *ei, int64_t *ej, double *w, int64_t *output, size_t *outputlen)
{
    nverts = n + 2;
    nedges = m + 2 * n;
    int i;
    double (*edges_info)[3] = (double (*)[3])malloc(sizeof(double) * nedges * 3);
    if(edges_info == NULL)
    {
	cout << "malloc fail!" << endl;
    }
    double *degree = (double *)malloc(sizeof(double) * n);//degree of every vertex
    fill(degree, degree + n, 0);
    for(i = 0; i < m; i ++)
    {
	edges_info[i][0] = ei[i];
	edges_info[i][1] = ej[i];
	edges_info[i][2] = w[i];
	degree[(int)edges_info[i][0]] += edges_info[i][2];
	edges_info[i][0] ++;
	edges_info[i][1] ++;
    }
	
//define some necessary variables
double final_degree = 0;//the degree of the densest subgraph
double L = 0;//lower bound of Andrew Goldberg's algorithm
double U = m / 2;//upper bound of Andrew Goldberg's algorithm
int *final_cut = (int *)malloc(sizeof(int) * nverts);
//Andrew Goldberg's algorithm
int iter = 0;
while(n * (n - 1) * (U - L) >= 1)
{
//cout << n * (n - 1) * (U - L) << endl;
    iter ++;
    g = (U + L) / 2;
    src = 0;
    dest = nverts - 1;
    init_edges(edges_info, degree, n, m);
    free_malloc();
    max_flow(edges_info);
    if(accumulate(cut, cut + nverts, 0) == 1)
    {
	U = g;
    }
    else
    {
	L = g;
	for(i = 0; i < nverts; i ++)
	{
	    final_cut[i] = cut[i];
	}
    }
}

final_cut[0] = 0;
final_cut[nverts - 1] = 0;

//retrieve the densest subgraph from the final_cut
int num = 0;
for(i = 1; i < nverts - 1; i ++)
{
    if(final_cut[i] != 0)
    {
	output[num ++] = i - 1;
	for(int &e = pro3[i]; e >= 0; e = next[e])
	{
	    if(final_cut[to[e]] != 0)
	    {
	    final_degree ++;
	    }
	}
    }
}
final_degree /= (2 * num);
*outputlen = num;

free(final_cut);
free(degree);
free(edges_info);
return final_degree;
}


void init_edges(double (*edges_info)[3], double *degree, int64_t n, int64_t m)
{
    long i;
    for(i = m; i < m + n; i ++)
    {
	edges_info[i][0] = src;
	edges_info[i][1] = i - m + 1;
	edges_info[i][2] = m / 2;
    }
    for(i = n + m; i < m + 2 * n; i ++)
    {
	edges_info[i][0] = i - m - n + 1;
	edges_info[i][1] = dest;
	edges_info[i][2] = m / 2 + 2 * g - degree[i - m - n];
    }
}

void free_malloc()
{
    free(Q);
    free(fin);
    free(pro);
    free(another_pro);
    free(pro3);
    free(dist);
    free(flow);
    free(cap);
    free(next);
    free(to);
    free(cut);
}
