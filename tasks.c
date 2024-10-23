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
    /*  YOUR CODE HERE  */
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
    /*  YOUR CODE HERE  */
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
    /*  YOUR CODE HERE  */
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
    /*  YOUR CODE HERE  */
}
