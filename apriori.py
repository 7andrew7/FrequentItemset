#!/usr/bin/env python

import itertools

def apriori(baskets, support):
    """Compute frequent itemsets using the apriori algorithm.

    :param baskets: A dictionary mapping from a basket ID to an item set.
    Basket IDs and item IDs must be immutable and hashable.
    :param support: Support level, expressed as as a percentage of baskets.

    :return: A list of frequent item sets.  Element k contains a list of sets
    of length k+1.
    """
    outputs = []

    # Compute initial candidates: all items as singleton sets
    all_items = set()
    for itemset in baskets.itervalues():
        all_items.update(itemset)

    C = [frozenset([x]) for x in all_items]
    print C

if __name__ == '__main__':
    baskets = {
        'A': {4, 5, 2},
        'B': {7, 2, 3},
        'C': {3, 2, 1, 4, 7},
        'D': {2, 4, 5, 7},
        'E': {1, 2, 5},
        'F': {2, 4, 6, 7},
        'G': {1, 2, 3, 4, 5},
        'H': {2, 5, 6, 7},
    }

    apriori(baskets, .2)
