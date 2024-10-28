#include "helpers.h"
#include "tasks.h"

/*
 * TASK 0a
 *
 * Given n x n matrices A and B, this function calculates C = A*B, storing the
 * result in C.
 *
 * e.g., [1,2,3,4,5,6,7,8,9]  *  [1,2,3,4,5,6,7,8,9] =
 *       [30,36,42,66,81,96,102,126,150]
 *
 * This is the naive O(n^3) approach.
 *
 * PRECONDITIONS:  A, B, and C are n x n matrices in row-major order.
 * (They are arrays of n*n elements each.) C is initialized to zero.
 */
void matmult(int n, double* A, double* B, double* C) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                C[i * n + j] += A[i * n + k] * B[k * n + j];
            }
        }
    }
}

/*
 * TASK 0b
 *
 * As with matmult, this is the naive, O(n^3) algorithm. However, the `B`
 * matrix is in a *column-major* format. `A` and `C` remain in row-major
 * order.
 *
 * PRECONDITIONS:  A and C are n x n row-major matrices. B is an n x n
 * column-major matrix. C is initialized to zero.
 */
void matmult_cm(int n, double* A, double* B, double* C) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                C[i * n + j] += A[i * n + k] * B[j * n + k];
            }
        }
    }
}

/*
 * TASK 0c
 *
 * The original (all row-major) matrix multiplication algorithm, with the `j`
 * and `k` loops interchanged. This optimization eliminates the stride-n
 * access over B[k][j].
 *
 * To make this work, you will also need to refactor the bodies of the inner
 * and middle loops a little, so that the loops are perfectly nested.
 *
 * PRECONDITIONS:  A, B, and C are all n x n row-major matrices. C is
 * initialized to zero.
 */
void matmult_li(int n, double* A, double* B, double* C) {
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            for (int j = 0; j < n; j++) {
                C[i * n + j] += A[i * n + k] * B[k * n + j];
            }
        }
    }
}
/*
 * TASK 1
 *
 * Like `matmult`, but use blocking. The block size is `BLOCKSZ`.
 *
 * PRECONDITIONS:  A, B, and C are all n x n row-major matrices. C is
 * initialized to zero.
 */
void matmult_bl(int n, double* A, double* B, double* C) {
    int n_blocks = (n + BLOCKSZ - 1) / BLOCKSZ; // Ceiling division to cover all elements

    for (int ii = 0; ii < n_blocks; ii++) {
        int i_start = ii * BLOCKSZ;
        int i_end = (i_start + BLOCKSZ < n) ? (i_start + BLOCKSZ) : n;

        for (int jj = 0; jj < n_blocks; jj++) {
            int j_start = jj * BLOCKSZ;
            int j_end = (j_start + BLOCKSZ < n) ? (j_start + BLOCKSZ) : n;

            for (int kk = 0; kk < n_blocks; kk++) {
                int k_start = kk * BLOCKSZ;
                int k_end = (k_start + BLOCKSZ < n) ? (k_start + BLOCKSZ) : n;

                if (check_shortcircuit()) return;

                for (int i = i_start; i < i_end; i++) {
                    for (int j = j_start; j < j_end; j++) {
                        double sum = 0.0;
                        for (int k = k_start; k < k_end; k++) {
                            sum += A[i * n + k] * B[k * n + j];
                        }
                        C[i * n + j] += sum;
                    }
                }
            }
        }
    }
}

