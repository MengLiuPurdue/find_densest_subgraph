#ifndef _DEF_H
#define _DEF_H

double max_flow(double (*edges_info)[3], int64_t nverts, int64_t nedges, int64_t src, int64_t dest, int64_t *Q, int64_t *fin, int64_t *pro, int64_t *dist, int64_t *next, int64_t *to, int64_t *cut, int64_t *another_pro, int64_t *pro3, double *flow, double *cap);


#endif
