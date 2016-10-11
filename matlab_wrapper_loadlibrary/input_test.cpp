#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <inttypes.h>
#include <algorithm>
#include <vector>
#include <unordered_set>
#include "input_test.h"

using namespace std;

#define TRUE 0
#define FALSE 1

int check_symmetric(int64_t *ei, int64_t *ej, int64_t m, int64_t n)
{
    vector< unordered_set<int> > graph(n);

    for (int64_t i = 0; i < m; i ++) {
        graph[ei[i]].insert(ej[i]);
    }

    for (int64_t i = 0; i < m; i ++) {
        if (graph[ej[i]].count(ei[i]) == 0) {
            fprintf(stderr, "Symmetric Error in Line %ld\n!", i + 2);
            return FALSE;
        }
    }

    return TRUE;
}

int check_repeated(int64_t *ei, int64_t *ej, int64_t m, int64_t n)
{
    vector< unordered_set<int> > graph(n);

    for (int64_t i = 0; i < m; i ++) {
        if (graph[ei[i]].count(ej[i]) != 0) {
            fprintf(stderr, "Repeated Edges in Line %ld\n!", i + 2);
            return FALSE;
        }
        graph[ei[i]].insert(ej[i]);
    }

    return TRUE;
}

int check_diagonal(int64_t *ei, int64_t *ej, int64_t m)
{
    int64_t i;
    for(i = 0; i < m; i ++)
    {
        if(ei[i] == ej[i])
        {
            fprintf(stderr, "Diagonal Edge in Line %ld\n!", i + 2);
            return FALSE;
        }
    }
    return TRUE;
}

int input_test(int64_t n, int64_t m, int64_t *ei, int64_t *ej, double *w)
{
    if(check_diagonal(ei, ej, m))
    {
	return FALSE;
    }
    if(check_repeated(ei, ej, m, n))
    {
	return FALSE;
    }
    if(check_symmetric(ei, ej, m, n))
    {
	return FALSE;
    }
    return TRUE;
}

