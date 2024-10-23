#!/usr/bin/python3
"""Plot collected matrix multiply times.
"""

import argparse
import matplotlib.pyplot as plt
import csv
from collections import defaultdict


def load_csv(filename):
    """Load some experimental data from a CSV file.

    The result is a two-level dict, so `d[mat_size][block_size]` is the
    running time for a given configuration.
    """
    times_by_size = defaultdict(dict)
    with open(filename) as f:
        reader = csv.DictReader(f)
        for row in reader:
            mat_size = int(row["matrix size"])
            block_size = int(row["block size"])
            time = float(row["time"])
            times_by_size[mat_size][block_size] = time
    return times_by_size


def draw_plot(times_by_size, norm=False):
    plots = []
    for mat_size, times_by_block in times_by_size.items():
        if norm:
            # Normalize running time to the first block size (probably 1).
            baseline = list(times_by_block.values())[0]
            times_by_block = {b: t / baseline
                              for b, t in times_by_block.items()}
        p, = plt.plot(times_by_block.keys(), times_by_block.values())
        plots.append(p)

    plt.legend(plots, [f"{n} by {n}" for n in times_by_size.keys()])
    plt.xscale('log', base=2)

    plt.title('Block Size vs. Runtime')
    plt.xlabel('Block Size')
    if norm:
        plt.ylabel('Normalized Running Time')
    else:
        plt.ylabel('Running Time (ms)')
    plt.savefig("plot.png")  # TODO
    plt.show()


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--norm', action='store_true', default=False,
                        help='Normalize running times')
    args = parser.parse_args()

    runtimes = load_csv("runtimes.csv")
    draw_plot(runtimes, args.norm)
