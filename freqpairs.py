from itertools import combinations
from collections import Counter

def freqpairs_brute_force(it, support):
    """Compute frequent pairs through brute force enumeration.

    :param it: A basket iterator containing tuples.
    :param support: Required support level as an integer count
    :return: A subset of it contain frequent pairs, represented as an iterator.
    """

    counter = Counter((x, y) for tpl in it for x, y in combinations(tpl, 2))
    return (key for key, val in counter.iteritems() if val >= support)

def freqpairs_apriori(it, support):
    counter1 = Counter(x for tpl in it for x in tpl)
    l1 = (key for key, val in counter1.iteritems() if val >= support)

    c2 = set(combinations(l1, 2))
    counter2 = Counter((x,y) for tpl in it for x, y in combinations(tpl, 2) if (x, y) in c2)
    return (key for key, val in counter2.iteritems() if val >= support)

if __name__ == '__main__':
    _in = [
        (2, 5, 6),
        (2, 3, 7, 8),
        (1, 2, 3, 4, 7),
        (2, 4, 7, 9),
        (1, 2, 5, 9),
        (2, 4, 6, 7),
        (1, 2, 3, 5, 7),
        (2, 5, 6, 7)
    ]

    fp = freqpairs_apriori(_in, 2)
    print sorted(list(fp))
