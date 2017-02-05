#!/usr/bin/env python

import argparse
import random

minValue = -50.0
maxValue = 50.0

def rand(min, max):
    return random.random() * (max - min) + min

if __name__ == "__main__":
    description = "A script for generating test data in text format"
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument("filename",
                        help="Output filename")
    parser.add_argument("count",
                        type=int,
                        help="The number of records to generate")
    parser.add_argument("start_index",
                        nargs='?',
                        type=int,
                        default=1,
                        help="The minimal value of indexes in the data")
    args = parser.parse_args()

    with open(args.filename, 'w') as f:
        for i in range(args.start_index, args.count + args.start_index):
            f.write("{i},{x},{y}\n".format(i=i,
                                         x=rand(minValue, maxValue),
                                         y=rand(minValue, maxValue)))
