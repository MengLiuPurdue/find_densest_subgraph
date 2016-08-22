#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include "def.h"

using namespace std;

void new_edge(int64_t u, int64_t v, double weight, int64_t *to, double *cap, double *flow, int64_t *next, int64_t *fin, long *nEdge)
{
    to[*nEdge] = v;
    cap[*nEdge] = weight;
    flow[*nEdge] = 0;
    next[*nEdge] = fin[u];
    fin[u] = (*nEdge) ++;
    to[*nEdge] = u;
    cap[*nEdge] = weight;
    flow[*nEdge] = weight;
    next[*nEdge] = fin[v];
    fin[v] = (*nEdge) ++;
}

bool dinic_bfs(int64_t nverts, int64_t src, int64_t dest, int64_t *dist, int64_t *Q, int64_t *fin, int64_t *next, int64_t *to, double *flow, double *cap) 
{
    int64_t st, en, i, u, v;
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

double dinic_dfs(int64_t u, double fl, int64_t src, int64_t dest, int64_t *pro, int64_t *next, int64_t *to, int64_t *dist, double *cap, double *flow) 
{
    if(u == dest) return fl;
    int64_t v;
    double df;
    for(int64_t &e=pro[u]; e >= 0; e = next[e]) 
    {
    v = to[e];
    if(flow[e] < cap[e] && dist[v] == dist[u] + 1) 
    {
        if(u == src || (cap[e] - flow[e]) <= fl)
        {
        fl = cap[e] - flow[e];
        }
        df = dinic_dfs(v, fl, src, dest, pro, next, to, dist, cap, flow);
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


void find_cut(int64_t u, int64_t *cut, int64_t *another_pro, int64_t *next, int64_t *to, double *flow, double *cap)
{
    cut[u] = 1;
    for(int64_t &e = another_pro[u]; e >= 0; e = next[e])
    {
    int64_t v = to[e];
    if(flow[e] < cap[e] && cut[v] == 0)
    {
        find_cut(v, cut, another_pro, next, to, flow, cap);
    }
    }
}


double max_flow(double (*edges_info)[3], int64_t nverts, int64_t nedges, int64_t src, int64_t dest, int64_t *Q, int64_t *fin, int64_t *pro, int64_t *dist, int64_t *next, int64_t *to, int64_t *cut, int64_t *another_pro, int64_t *pro3, double *flow, double *cap)
{
    long i;
    fill(fin, fin + nverts, -1);
    fill(cut, cut + nverts, 0);
    long nEdge = 0;
    for(i = 0; i < nedges; i ++)
    {
    new_edge(edges_info[i][0], edges_info[i][1], edges_info[i][2], to, cap, flow, next, fin, &nEdge);
    }

    double ret = 0;
    double df;
    while(dinic_bfs(nverts, src, dest, dist, Q, fin, next, to, flow, cap)) 
    {
    for(i = 0; i < nverts; i++) 
    {
        pro[i] = fin[i];
        another_pro[i] = fin[i];
        pro3[i] = fin[i];
    }
    while(true) 
    {
        df = dinic_dfs(src, 0, src, dest, pro, next, to, dist, cap, flow);
        if(df) ret += df;
        else break;
    }
    }
    find_cut(src, cut, another_pro, next, to, flow, cap);
    return ret;
}
