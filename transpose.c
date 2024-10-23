/*
 *  transpose.c
 *  CS3410 (F'24)
 *
 *  USAGE:  transpose  <matrix_dimension>
 *
 *  OVERVIEW:  Driver to test the performance of various cache-aware optimizations
 *    on the basic matrix transposition algorithm.
 *
 *  AUTHOR:  John H. E. Lasseter (jhl287)
 */

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "helpers.h"

int main(int argc, char **argv) {
    long long start_blocked, end_blocked;
    long long start_basic, end_basic;
    unsigned long total_basic, total_blocked; // The time to completion of A*B = C, in seconds

    int n = get_arg(argc, argv);
    bool verbose = n <= 16; // Should we display the matrix calculation, too?

    printf("\n(creating test matrices ...");
    fflush(stdout);
    double *M = make_one_matrix(n);
    double *M_t = make_one_matrix(n);

    printf(" performing benchmark ...");
    fflush(stdout);

    start_basic = timeInMilliseconds();
    transpose(n, M, M_t);
    end_basic = timeInMilliseconds();

    start_blocked = timeInMilliseconds();
    transpose_bl(n, M, M_t);
    end_blocked = timeInMilliseconds();

    printf(" done)\n\n");

    total_basic = end_basic - start_basic;
    total_blocked = end_blocked - start_blocked;
    // Not strictly valid timing, since we've including function call/return overhead.

    if (verbose) { // main
        // printf("----------------------------------------------------------\n");
        printf("\nM:\n");
        print_one_matrix(n, M, true);

        printf("\nM_t:\n");
        print_one_matrix(n, M_t, true);
    }

    printf("Time to calculate the transpose of a %d x %d matrix\n", n, n);
    print_results_transpose(n, BLOCKSZ, total_basic, total_blocked);

    // Loop interchange offers no benefit for transpose, but we might as well include it ...
    printf("\n----------------------------------------------------------\n");
    printf("   (results with loop interchange version)\n");
    printf("-------------------------------------\n");
    start_basic = timeInMilliseconds();
    transpose_li(n, M, M_t);
    end_basic = timeInMilliseconds();
    total_basic = end_basic - start_basic;
    if (verbose) { // main
        printf("\nM_t:\n");
        print_one_matrix(n, M_t, true);
    }
    printf("  TIME TO COMPLETION = %lu msec.\n\n\n", (total_basic));

} // main
