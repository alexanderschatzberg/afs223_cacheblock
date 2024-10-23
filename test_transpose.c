#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "helpers.h"

int main(int argc, char **argv) {
    double *M, *M_t;

    int n = get_arg(argc, argv);

    M = make_one_matrix(n);
    M_t = make_one_matrix(n);

    printf("Testing transpose of  %d x %d matrix M:\n", n, n);
    print_one_matrix(n, M, true);

    transpose(n, M, M_t);

    printf("\nM_t:\n");
    print_one_matrix(n, M_t, true);

    printf("\n----------------------------\n");
    printf("With blocking (block size=%d,%sshortcircuit):\n", BLOCKSZ, (SHORT_CIRCUIT ? "" : "no "));
    zero(n, M_t);

    transpose_bl(n, M, M_t);
    print_one_matrix(n, M_t, true);

    return 0;
} // main
