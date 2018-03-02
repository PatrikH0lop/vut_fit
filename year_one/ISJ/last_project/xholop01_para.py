#!/usr/bin/env python3

from multiprocessing import Pool

def count(n):
    while n > 0:
        n -= 1

multiprocess = Pool(2)
multiprocess.map(count, [10**8, 10**8])

