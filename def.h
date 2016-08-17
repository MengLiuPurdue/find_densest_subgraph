#ifndef _DEF_H
#define _DEF_H

double max_flow(double (*)[3]);
extern int nverts;
extern int ncols;
extern int nedges; 
extern int src;
extern int dest;
extern int *Q, *fin, *pro, *dist, *next, *to, *cut, *another_pro, *pro3;
extern double *flow, *cap;

#endif
