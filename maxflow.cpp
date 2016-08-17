#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include "def.h"

using namespace std;

/*struct Edge{
	int to;
	int num;
	double weight, flow;
};*/

long nEdge;
int *Q, *fin, *pro, *dist, *next, *to, *cut, *another_pro, *pro3;
double *flow, *cap;

void new_edge(int u, int v, double weight)
{
    to[nEdge] = v;
    cap[nEdge] = weight;
    flow[nEdge] = 0;
    next[nEdge] = fin[u];
    fin[u] = nEdge ++;
    to[nEdge] = u;
    cap[nEdge] = weight;
    flow[nEdge] = weight;
    next[nEdge] = fin[v];
    fin[v] = nEdge ++;
}

bool dinic_bfs() 
{
    int st, en, i, u, v;
    fill(dist, dist + nverts, -1);
    dist[src] = st = en = 0;
    Q[en ++] = src;
    while(st < en) 
    {
	u = Q[st ++];
        for(i = fin[u]; i >= 0; i = next[i]) 
	{
	    v = to[i];
            if(flow[i] < cap[i] && dist[v] == -1) 
	    {
                dist[v] = dist[u] +1;
                Q[en ++] = v;
            }
        }
    }
    return dist[dest] != -1;
}

double dinic_dfs(int u, double fl) 
{
    if(u == dest) return fl;
	int v;
	double df;
    for(int &e=pro[u]; e >= 0; e = next[e]) 
    {
        v = to[e];
        if(flow[e] < cap[e] && dist[v] == dist[u] + 1) 
	{
            if(u == src || (cap[e] - flow[e]) <= fl)
	    {
		fl = cap[e] - flow[e];
	    }
	    df = dinic_dfs(v, fl);
            if(df>0) 
	    {
                flow[e] += df;
                flow[e^1] -= df;
                return df;
            }
        }
    }
    return 0;
}


void find_cut(int u)
{
    cut[u] = 1;
    for(int &e = another_pro[u]; e >= 0; e = next[e])
    {
	int v = to[e];
	if(flow[e] < cap[e] && cut[v] == 0)
	{
	    find_cut(v);
	}
    }
}


double max_flow(double (*edges_info)[3])
{
    Q = (int *)malloc(sizeof(int) * nverts);
    fin = (int *)malloc(sizeof(int) * nverts);
    pro = (int *)malloc(sizeof(int) * nverts);
    another_pro = (int *)malloc(sizeof(int) * nverts);
    pro3 = (int *)malloc(sizeof(int) * nverts);
    dist = (int *)malloc(sizeof(int) * nverts);
    flow = (double *)malloc(sizeof(double) * 2 * nedges);
    cap = (double *)malloc(sizeof(double) * 2 * nedges);
    next = (int *)malloc(sizeof(int) * 2 * nedges);
    to = (int *)malloc(sizeof(int) * 2 * nedges);
    cut = (int *)malloc(sizeof(int) * nverts);
    long i;
    fill(fin, fin + nverts, -1);
    fill(cut, cut + nverts, 0);
    nEdge = 0;
    for(i = 0; i < nedges; i ++)
    {
	new_edge(edges_info[i][0], edges_info[i][1], edges_info[i][2]);
    }

    double ret = 0;
    double df;
    while(dinic_bfs()) 
    {
	for(i = 0; i < nverts; i++) 
	{
	    pro[i] = fin[i];
	    another_pro[i] = fin[i];
	    pro3[i] = fin[i];
	}
        while(true) 
	{
            df = dinic_dfs(src, 0);
            if(df) ret += df;
            else break;
        }
    }
    find_cut(src);
    return ret;
}
