#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "helpers.h"

void printUsage(char *progname) {
    fprintf(stderr, "USAGE:  %s <dimension> \n", progname);
    fprintf(stderr, "<dimension> must be a positive integer.\n");
} // printUsage

/*
 * Reads command line arguments for the matrix row/column dimension and the
 * block size to use.  If there are fewer than two command line arguments or
 * if the arguments are not positive integers, the program exits with a use
 * message;
 */
int get_arg(int argc, char **argv) {

    if (argc < 2) {
        printUsage(argv[0]);
        exit(0);
    }

    int dimension = atoi(argv[1]); // row & column count

    if (!dimension) {
        fprintf(stderr, "Bad dimension value %s.\n", argv[1]);
        printUsage(argv[0]);
        exit(0);
    }

    return dimension;
} // get_arg

/*
 * Constructs and returns a square, n x n matrix of floating point values,
 * stored sequentially, in row-major order.  For example, the matrix
 *
 *   1  2  3
 *   4  5  6
 *   7  8  9
 *
 * will be [1,2,3,4,5,6,7,8,9].  Returns the constructed matrix.
 * Note:  the result is dynamically allocated, so the caller is responsible
 * for garbage collection.
 *
 * Preconditions:  n > 0
 */
double *make_one_matrix(int n) {
    // assert(n > 0);

    double *M = (double *)malloc(n * n * sizeof(double));
    double ct = 1;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            M[i * n + j] = ct; //(ct/100.0));  // M[i][j] = ...
            ct++;
        }
    }
    return M;
} // make_one_matrix

/*
 * Sets the value of every cell in M to 0.0.
 */
void zero(int n, double *M) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            M[i * n + j] = 0.0;
        }
    }
}

/*
 * Displays the total time to calculate C = A*B. If verbose is true, it will also
 * display the contents of A, B, and C
 */
void print_results_transpose(int n, int blocksz, unsigned long interval_basic, unsigned long interval_blocked) {
    printf("\n----------------------------------------------------------\n");
    printf("   (results with naive version)\n");
    printf("-------------------------------------\n");
    printf("  TIME TO COMPLETION = %lu msec.\n\n\n", (interval_basic));

    printf("-------------------------------------\n");
    printf("   (results with blocking)\n");
    printf("-------------------------------------\n");
    printf("  N\t= %d\n", n);
    if (blocksz > 1) {
        printf("  BLOCK\t= %d\n", blocksz);
    } else {
        printf("  (no blocking)\n");
    }
    printf("\n");
    // printf("TIME TO COMPLETION = %lu msec.\n\n\n",(interval));
    printf("  TIME TO COMPLETION = %lu msec.\n\n\n", (interval_blocked));
} // print_results

/*
 * Displays the total time to calculate C = A*B. If verbose is true, it will also
 * display the contents of A, B, and C.  Includes the realigned results for both
 * basic and blocked versions.
 */
void print_results_matmult(int blocksz, MMTotals mmt) {
    printf("---------------------------------------------------------\n");
    if (blocksz > 1) {
        printf("  block size\t= %d\n", blocksz);
    } else {
        printf("  (no blocking)\n");
    }
    printf("---------------------------------------------------------\n");
    printf("  TIME TO COMPLETION (naive) = %lu msec.\n", (mmt.total_basic));
    printf("  TIME TO COMPLETION (realigned) = %lu msec.\n", (mmt.total_cm));
    printf("  TIME TO COMPLETION (interchange) = %lu msec.\n", (mmt.total_li));
    printf("  TIME TO COMPLETION (blocked) = %lu msec.\n", (mmt.total_bl));
    printf("---------------------------------------------------------\n\n");
} // print_results_full

/*
 * Nicely-formatted presentation of A * B = C.  Obviously, we assume the
 * contents of A,B, and C are compatible with this display.  All three are
 * assumed to be sequential representations in row-major order.
 */
void print_matrix_product(int n, double *A, double *B, double *C) {
    for (int row = 0; row < n; row++) {
        // A row
        printf("    | ");
        for (int col = 0; col < n; col++) {
            printf("%4.1f  ", A[row * n + col]);
        }

        // B row
        if (row == n / 2) {
            printf("|  *  | ");
        } else {
            printf("|     | ");
        }

        for (int col = 0; col < n; col++) {
            printf("%4.1f  ", B[row * n + col]);
        }

        // C row
        if (row == n / 2) {
            printf("|  =  | ");
        } else {
            printf("|     | ");
        }

        for (int col = 0; col < n; col++) {
            printf("%7.1f  ", C[row * n + col]);
        }

        printf("|\n");
    }
} // print_matrix_product

/*
 * Nicely-formatted display of M, which is assumed to be an n x n matrix.
 *
 *   1  2  3
 *   4  5  6
 *   7  8  9
 *
 * Parameters:
 *   n:  the numbers of rows/coluns in M
 *   M:  a n*n matrix, represented as an array of length (n*n)
 *   r_major: true if and only if M is stored in row-major order
 *
 * Preconditions:  n > 0 && M is an (n*n) length array
 */
void print_one_matrix(int n, double *M, bool r_major) {
    for (int row = 0; row < n; row++) {
        printf("    | ");
        for (int col = 0; col < n; col++) {
            if (r_major)
                printf("%7.1f   ", M[row * n + col]);
            else
                printf("%7.1f   ", M[row + col * n]);
        }
        printf("|\n");
    }
} // print_one_matrix

/*
 * Prints the raw sequential representation of M, an n x n matrix.  Might be useful
 * for debugging.
 *
 * Parameters:
 *   n:  the numbers of rows/coluns in M
 *   M:  a n*n matrix, represented as an array of length (n*n)
 *
 * Preconditions:  n > 0 && M is an (n*n) length array
 */
void print_matrix_linear(int n, double *M) {
    int i;
    printf("[");
    for (i = 0; i < (n * n - 1); i++) {
        printf("%5.1f,", M[i]);
    }
    printf("%5.1f]\n", M[i]);
}

/*
 * Get the current time, in milliseconds.  Taken from a SE comment at:
 *
 * https://stackoverflow.com/a/44896326
 */
long long timeInMilliseconds() {
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return (((long long)tv.tv_sec) * 1000) + (tv.tv_usec / 1000);
} // timeInMilliseconds

/////////////////////////////////////////////////////////////////////////
/* 
 * A constant, a global variable, and a simple utility method, which aid
 * in testing your blocked implementations. An example usage is in the
 * transpose_blocked function, below.
 */
static const int MAX_SHORTCIRCUIT = 2;
static int Blocks_Remaining = MAX_SHORTCIRCUIT; // Short-circuit limit

bool check_shortcircuit() {
    if (SHORT_CIRCUIT && Blocks_Remaining == 0) {
        Blocks_Remaining = MAX_SHORTCIRCUIT;
        return true;
    }
    Blocks_Remaining--;
    return false; // More compact ways of writing this are obfuscated
}
/////////////////////////////////////////////////////////////////////////

/*
 * Transposes M, which is assumed to be a sequential representation of an n x n matrix.
 * This transforms M from a row-major representation to column-major, and vice versa.
 * For example, the 3x3 matrix M
 *
 *   1  2  3                  1  4  7
 *   4  5  6       becomes    2  5  8
 *   7  8  9                  3  6  9
 *
 * i.e., [1,2,3,4,5,6,7,8,9]  ===>  [1,4,7,2,5,8,3,6,9]
 *
 * The transposed matrix will be stored in M_t
 *
 */
void transpose(int n, double *M, double *M_t) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            M_t[j * n + i] = M[i * n + j]; // M_t[j][i] = M[i][j]
        }
    }
} // transpose

/*
 * The same thing as transpose(), but with the loops over i and j switched.
 */
void transpose_li(int n, double *M, double *M_t) {
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            M_t[j * n + i] = M[i * n + j];
        }
    }
} // transpose_li

/*
 * Calculates the transpose of M, which is stored in M_t.
 * This adds to the naive approach the ability to calculate the transpose in blocks,
 * as determined by the value of BLOCKSZ.
 *
 * NOTE:  Unlike the matrix multiplication function (dgemm_blocked), there isn't as much
 * benefit to blocking here, as there really is no way to eliminate the stride-n access
 * pattern in M or M_t.
 */
void transpose_bl(int n, double *M, double *M_t) {
    // Compiler optimizations do a better job with a hard constant for this value.  For now,
    // I'm going to leave this here instead of putting the constant everywhere, in case
    // someone wants to experiment with dynamically-set values for the block size.
    int N = (n % BLOCKSZ == 0 ? (n / BLOCKSZ) : (n / BLOCKSZ) + 1);
    // # of block rows & block columns
    // (gymnastics to handle the case where BLOCKSZ does not evenly divide n)

    for (int ii = 0; ii < N; ii++) {     // for every row block
        for (int jj = 0; jj < N; jj++) { // every column block

            if (check_shortcircuit()) return;
            // (beginning of adapted transpose() body)
            for (int i = ii * BLOCKSZ; (i < (ii + 1) * BLOCKSZ) && (i < n); i++) {
                for (int j = jj * BLOCKSZ; (j < (jj + 1) * BLOCKSZ) && (j < n); j++) {
                    M_t[j * n + i] = M[i * n + j];
                }
            }
            // (end of adapted transpose() body)
        }
    }
} // transpose_blocked
