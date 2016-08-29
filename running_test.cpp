#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <inttypes.h>
#include <algorithm>
#include <vector>
#include <unordered_set>

using namespace std;

#include "def.h"

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


int main(int argc, char* argv[])
{
    FILE *rptr = fopen(argv[1], "r");
    if(rptr == NULL) {
        fprintf(stderr, 
                "The file %s couldn't be opened for reading (Does it exist?)!\n", 
                argv[1]);
        return EXIT_FAILURE;
    }

    /*read the content from the input file to a stringstream*/
    fseek(rptr, 0, SEEK_END);
    size_t fsize = ftell(rptr);
    char *read_file = (char *)malloc(sizeof(char) * fsize);
    if(read_file == NULL) {
        fprintf(stderr, "malloc failing!\n");
        fclose(rptr);
        return EXIT_FAILURE;
    }
    fseek(rptr, 0, SEEK_SET);
    size_t nread = fread(read_file, sizeof(char), fsize, rptr);
    if (nread != fsize) {
        fprintf(stderr, "read failed on %s\n", argv[1]);
        return EXIT_FAILURE;
    }
    stringstream ss;
    ss << read_file;
    free(read_file);

    int64_t n, m;
    int64_t *ei, *ej;
    double *w;

    ss >> n;
    ss >> n;
    ss >> m;
    ei = (int64_t *)malloc(sizeof(int64_t) * m);
    ej = (int64_t *)malloc(sizeof(int64_t) * m);
    w = (double *)malloc(sizeof(double) * m);
    int64_t i;
    for(i = 0; i < m; i ++) {
        ss >> ei[i];
        ss >> ej[i];
        ss >> w[i];
        // TODO validate input
        if(ei[i] < 0 || ei[i] >= n || ej[i] < 0 || ej[i] >= n)
        {
            fprintf(stderr, "Invalid Input in Line %ld!\n", i + 2);
            fclose(rptr);
            free(ei);
            free(ej);
            free(w);
            return EXIT_FAILURE;
        }
        // TODO check for negative weights
        if(w[i] < 0)
        {
            fprintf(stderr, "Negative Weight in Line %ld!\n", i + 2);
            fclose(rptr);
            free(ei);
            free(ej);
            free(w);
            return EXIT_FAILURE;
        }
    }
    fclose(rptr);

    // TODO check for diagonal edges
    if(check_diagonal(ei, ej, m))
    {
        free(ei);
        free(ej);
        free(w);
        return EXIT_FAILURE;
    }

    // TODO check for repeated edges
    if(check_repeated(ei, ej, m, n))
    {
        free(ei);
        free(ej);
        free(w);
        return EXIT_FAILURE;
    }
    // TODO check symmetric
    if(check_symmetric(ei, ej, m, n))
    {
        free(ei);
        free(ej);
        free(w);
        return EXIT_FAILURE;
    }




    fprintf(stderr, "loaded file %s : %12" PRId64 " nodes   %12" PRId64 " edges\n",
            argv[1], n, m);

    double ret;
    int64_t *output = (int64_t *)malloc(sizeof(int64_t) * n);
    size_t outputlen = n;
    ret = densest_subgraph(n, m, ei, ej, w, output, &outputlen);
    cout << ret << endl;
    cout << outputlen << endl;
    for (size_t j = 0; j < outputlen; ++j) {
        cout << output[j] << endl;
    }

    return 0;
}
