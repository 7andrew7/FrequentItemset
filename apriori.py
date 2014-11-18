#!/usr/bin/env python

import itertools
import math
import collections

def is_sorted(ls):
    return all(ls[i] <= ls[i+1] for i in xrange(len(ls) - 1))

def candidate_gen(frequent_itemsets):
    """Compute new candidates of length K.    

    Input is a list of item set tuples of length K-1.
    Output is a list of item set tuples of length K.
    """

    C = []

    # Join pairs of itemsets to construct candidates
    for c0, c1 in itertools.combinations(frequent_itemsets, 2):
        assert is_sorted(c0)
        assert is_sorted(c1)
        
        if c0[:-1] == c1[:-1]:
            _max = max(c0[-1], c1[-1])
            _min = min(c0[-1], c1[-1])
            C.append(c0[:-1] + (_min, _max))

    # Prune candidates with an unsupported subset
    C2 = []
    for c in C:
        for subset in itertools.combinations(c, len(c) - 1):
            if tuple(subset) not in frequent_itemsets:
                break
        else:
            C2.append(c)

    return C2

def contains(t1, t2):
    """Return True if the elements of t1 are contained in t2.

    Assumes sorted tuples.
    """
    p2 = 0
    for v1 in t1:
        for i in range(p2, len(t2)):
            if t2[i] == v1:
                p2 = i + 1
                break
            if t2[i] > v1:
                return False
        else:
            return False # advanced through the list

    return True

def apriori(baskets, support):
    """Compute frequent itemsets using the apriori algorithm.

    :param baskets: A list of item set tuples.
    :param support: Support level, expressed as as a percentage of baskets.

    :return: A list of frequent item sets.  Element k contains a list of sets
    of length k+1.
    """

    # Compute initial candidates: all items as singleton sets
    all_items = set()
    for itemset in baskets:
        all_items.update(itemset)

    C = [(x,) for x in all_items]

    outputs = []
    threshold = int(math.ceil(support * len(baskets)))

    while True:
        # Compute frequent itemsets from among the candidate sets
        counts = collections.Counter()

        for itemset in baskets:
           for candidate in C:
                if contains(candidate, itemset):
                    counts[candidate] += 1

        F = [candidate for (candidate, count) in counts.iteritems()
             if count >= threshold]

        if len(F) == 0:
            return outputs
        outputs.append(F)

        # Create new candidates for the next iteration...
        C = candidate_gen(F)

if __name__ == '__main__':
    baskets = [
        (2, 5, 6),
        (2, 3, 7, 8),
        (1, 2, 3, 4, 7),
        (2, 4, 5, 7),
        (1, 2, 5),
        (2, 4, 6, 7),
        (1, 2, 3, 7, 5),
        (2, 5, 6, 7)
    ]

    output = apriori(baskets, .2)
    for i, sets in enumerate(output):
        print '################ %d ##############' %  (i + 1)
        print sets
