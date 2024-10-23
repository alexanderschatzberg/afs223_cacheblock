/*
 *  matmult.c
 *  CS3410 (F'24)
 *
 *  USAGE:  matmult  <matrix_dimension>
 *
 *  OVERVIEW:  Tests the performance of various cache-aware optimizations of matrix
 *     multiplication and reports the results.
 *
 *  AUTHOR:  John H. E. Lasseter (jhl287)
 */

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "helpers.h"
#include "tasks.h"

int main(int argc, char **argv) {
    double *A, *B, *C;
    long long start, end;
    MMTotals mmt;
    // The time to completion of A*B = C, in seconds

    int n = get_arg(argc, argv);

    bool verbose = n <= 8; // Should we display the matrix calculation, too?

    // Set up and run timing for unoptimized matrix multiplication:

    printf("\n(creating test matrices ...");
    fflush(stdout);
    A = make_one_matrix(n);
    B = make_one_matrix(n);
    C = make_one_matrix(n);

    printf(" performing benchmark ...");
    fflush(stdout);

    zero(n, C); // All `matmult*` functions expect C to be initialized to 0.
    start = timeInMilliseconds();
    matmult(n, A, B, C);
    end = timeInMilliseconds();
    mmt.total_basic = end - start;

    zero(n, C);
    start = timeInMilliseconds();
    matmult_li(n, A, B, C);
    end = timeInMilliseconds();
    mmt.total_li = end - start;

    // Now try with B realigned to column-major representation:
    double *B_t = make_one_matrix(n);
    zero(n, B_t);
    transpose(n, B, B_t);

    zero(n, C);
    start = timeInMilliseconds();
    matmult_cm(n, A, B_t, C);
    end = timeInMilliseconds();
    mmt.total_cm = end - start;

    zero(n, C);
    start = timeInMilliseconds();
    matmult_bl(n, A, B, C);
    end = timeInMilliseconds();
    mmt.total_bl = end - start;

    printf(" done)\n\n");

    // Not strictly valid timing, since we're including function call/return overhead.
    printf("Time to calculate A*B = C, for %d x %d matrices A and B\n", n, n);
    if (verbose) {
        // printf("----------------------------------------------------------\n");
        printf("\n");
        print_matrix_product(n, A, B, C);
        printf("\n");
    }

    print_results_matmult(BLOCKSZ, mmt);

    return 0;
} // main
