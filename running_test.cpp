#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <inttypes.h>
#include <algorithm>
#include <unordered_set>

using namespace std;

#include "def.h"

#define PASS 0
#define FAIL 1

class Edge
{
    public:
        int64_t from;
        int64_t to;
};

struct EdgeHash
{
    size_t operator()(const pair<int64_t, int64_t>& edge) const 
    {
        auto h1 = hash<int64_t>{}(edge.first);
        auto h2 = hash<int64_t>{}(edge.second);
        return h1 ^ h2;
    }
};

int check_symmetric(int64_t *ei, int64_t *ej, int64_t m, unordered_set<pair<int64_t, int64_t>, EdgeHash> *edges_set)
{
    int64_t i;
    for(i = 0; i < m; i ++)
    {
        if(edges_set->find(make_pair(ej[i], ei[i])) == edges_set->end())
        {
            fprintf(stderr, "Symmetric Error!\n");
            return FAIL;
        }
    }
    return PASS;
}

int check_repeated(int64_t *ei, int64_t *ej, int64_t m, unordered_set<pair<int64_t, int64_t>, EdgeHash> *edges_set)
{
    int64_t i;
    for(i = 0; i < m; i ++)
    {
        if(edges_set->count(make_pair(ei[i], ej[i])))
        {
            fprintf(stderr, "Repeated Edge in Line %ld!\n", i + 2);
            return FAIL;
        }
        edges_set->insert(make_pair(ei[i], ej[i]));
    }
    return PASS;
}

int check_diagonal(int64_t *ei, int64_t *ej, int64_t m)
{
    int64_t i;
    for(i = 0; i < m; i ++)
    {
        if(ei[i] == ej[i])
        {
            fprintf(stderr, "Diagonal Edge in Line %ld!", i + 2);
            return FAIL;
        }
    }
    return PASS;
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

    unordered_set<pair<int64_t, int64_t>, EdgeHash> *edges_set = new unordered_set<pair<int64_t, int64_t>, EdgeHash>;
    // TODO check for repeated edges
    if(check_repeated(ei, ej, m, edges_set))
    {
        delete edges_set;
        free(ei);
        free(ej);
        free(w);
        return EXIT_FAILURE;
    }
    // TODO check symmetric
    if(check_symmetric(ei, ej, m, edges_set))
    {
        delete edges_set;
        free(ei);
        free(ej);
        free(w);
        return EXIT_FAILURE;
    }
    delete edges_set;




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
