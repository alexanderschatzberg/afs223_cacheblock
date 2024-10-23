#!/usr/bin/python3
"""Collect matrix multiplication running times for different block and
matrix sizes.
"""

import argparse
import subprocess
import shlex
import os
import re
import csv


# A prefix for running commands inside the CS 3410 container, like the
# course's `rv` alias.
RV = ["docker", "run", "-i", "--rm", "-v", f"{os.getcwd()}:/root",
      "ghcr.io/sampsyo/cs3410-infra"]


def get_stats(log, key):
    """Extract the running time for a given configuration from the
    output of a `matmult` execution.

    We assume that the value appears directly after the `=` symbol (and
    possibly some whitespace. The output looks like this:

        TIME TO COMPLETION (blocked) = 66 msec.

    Here, we would return the value `66` if the key is `(blocked)`.
    """

    for line in log.split('\n'):
        if key in line.split():
            match = re.search(r'=\s*(\d+)', line)
            assert match
            return float(match.group(1))
    return 0.0


def run_exp(mat_size, emulation=True):
    """Run a single experiment, using a pre-built executable.

    Execute the `matmult` binary with the given matrix size. Return the
    full standard output from the command.
    """
    if emulation:
        cmd = RV + ["qemu", "matmult", str(mat_size)]
    else:
        cmd = ["./matmult", str(mat_size)]
    print(shlex.join(cmd))
    proc = subprocess.run(cmd, stdout=subprocess.PIPE)
    return proc.stdout.decode()


def collect_times(matrix_sizes, block_sizes, emulation=True):
    """Run a series of experiments for different matrix and block sizes.

    Generate (matrix size, block size, running time) tuples.
    """
    # For each block size choice, we'll need to re-compile:
    for block_size in block_sizes:
        subprocess.run(["make", "clean"])

        make_cmd = ["make", "matmult", f"BLOCK={block_size}"]
        if emulation:
            make_cmd = RV + make_cmd
        subprocess.run(make_cmd)

        # Now run the resulting executable for this block size on the
        # various choices of matrix size.
        for mat_size in matrix_sizes:
            if mat_size >= block_size:
                # Skip cases where block size is too big.
                log = run_exp(mat_size, emulation)
                time = get_stats(log, '(blocked)')
                yield mat_size, block_size, time


def emit_csv(runtimes, filename):
    """Generate a CSV with the data returned from `collect_times`.
    """
    with open(filename, 'w') as f:
        writer = csv.writer(f)
        writer.writerow(["matrix size", "block size", "time"])
        for mat_size, block_size, time in runtimes:
            writer.writerow([mat_size, block_size, time])


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--native', action='store_true', default=False,
                        help='Compile for the host, not RISC-V')
    parser.add_argument('--matrix-sizes', '-n', required=True,
                        help='Comma-separated list of matrix sizes')
    parser.add_argument('--block-sizes', '-b', required=True,
                        help='Comma-separated list of block sizes')
    args = parser.parse_args()

    runtimes = collect_times(
        [int(s) for s in args.matrix_sizes.split(",")],
        [int(s) for s in args.block_sizes.split(",")],
        not args.native,
    )
    emit_csv(runtimes, "runtimes.csv")
