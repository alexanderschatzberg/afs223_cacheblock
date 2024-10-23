#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "helpers.h"
#include "tasks.h"

int main(int argc, char **argv) {
    double *A, *B, *C;

    int n = get_arg(argc, argv);

    A = make_one_matrix(n);
    B = make_one_matrix(n);
    C = make_one_matrix(n);

    double *B_t = make_one_matrix(n);
    zero(n, B_t);
    transpose(n, B, B_t);
    // Use B_t one for the versions that assume a realigned multiplicand

    zero(n, C);
    matmult(n, A, B, C);
    // Not strictly valid timing, since we're including function call/return overhead.
    printf("Testing calculation of A*B = C, for %d x %d matrices A and B\n", n, n);
    print_matrix_product(n, A, B, C);

    printf("\n----------------------------\n");
    printf("With column-major realignment (no blocking):\n");

    zero(n, C);
    matmult_cm(n, A, B_t, C);
    print_one_matrix(n, C, true);

    printf("\n----------------------------\n");
    printf("With loop interchange (no blocking):\n");

    zero(n, C);
    matmult_li(n, A, B, C);
    print_one_matrix(n, C, true);

    printf("\n--------------------------------------------------------\n");
    printf("BLOCK SIZE = %d\n", BLOCKSZ);
    printf("\n----------------------------\n");
    printf("With blocking (%sshortcircuit):\n", (SHORT_CIRCUIT ? "" : "no "));

    zero(n, C);
    matmult_bl(n, A, B, C);
    print_one_matrix(n, C, true);

    return 0;
} // main
