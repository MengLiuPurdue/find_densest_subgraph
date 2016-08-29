#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <inttypes.h>

using namespace std;

#include "def.h"

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
    }
    
    // TODO check symmetric
    // TODO check for diagonal edges
    // TODO check for negative weights
    // TODO validate input
    // TODO check for repeated edges
    
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
