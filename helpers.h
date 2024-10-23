#include <stdbool.h>

#ifndef BLOCKSZ
#define BLOCKSZ 1
#endif

#ifndef SHORT_CIRCUIT
#define SHORT_CIRCUIT 0
#endif

#ifndef BLOCKING_H
#define BLOCKING_H

typedef struct mmt_t {
    unsigned long total_basic; // naive matrix multiplication runtime
    unsigned long total_cm;    // runtime for multiplicand realignment
    unsigned long total_li;    // runtime for loop interchange
    unsigned long total_bl;    // runtime for blocking
} MMTotals;

void printUsage(char *);
int get_arg(int, char **);
double *make_one_matrix(int);
void zero(int, double *M);

void print_results_transpose(int, int, unsigned long, unsigned long);
void print_results_matmult(int, MMTotals);
void print_matrix_product(int, double *, double *, double *);

void print_one_matrix(int, double *, bool);
void print_matrix_linear(int, double *);

long long timeInMilliseconds(void);

bool check_shortcircuit(void);
void transpose(int n, double *M, double *M_t);
void transpose_li(int n, double *M, double *M_t);
void transpose_bl(int n, double *M, double *M_t);

#endif
