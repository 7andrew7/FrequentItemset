#!/usr/bin/env python

import math
import collections

def apriori(baskets, support):
    """Compute frequent itemsets using the apriori algorithm.

    :param baskets: A dictionary mapping from a basket ID to an item set.
    Basket IDs and item IDs must be immutable and hashable.
    :param support: Support level, expressed as as a percentage of baskets.

    :return: A list of frequent item sets.  Element k contains a list of sets
    of length k+1.
    """

    # Compute initial candidates: all items as singleton sets
    all_items = set()
    for itemset in baskets.itervalues():
        all_items.update(itemset)

    C = [frozenset([x]) for x in all_items]

    outputs = []
    threshold = int(math.ceil(support * len(baskets)))


    while True:
        print C

        # Compute frequent itemsets from among the candidate sets
        counts = collections.Counter()

        for candidate in C:
            for itemset in baskets.itervalues():
                if itemset.issuperset(candidate):
                    counts[candidate] += 1


        print counts

        F = [candidate for (candidate, count) in counts.iteritems() if count >= threshold]
        outputs.append(F)
        return outputs

        # Construct new candidate list

    return outputs

if __name__ == '__main__':
    baskets = {
        'A': {4, 5, 2},
        'B': {7, 2, 3, 8},
        'C': {3, 2, 1, 4, 7},
        'D': {2, 4, 5, 7},
        'E': {1, 2, 5},
        'F': {2, 4, 6, 7},
        'G': {1, 2, 3, 4, 5},
        'H': {2, 5, 6, 7},
    }

    print apriori(baskets, .25)
