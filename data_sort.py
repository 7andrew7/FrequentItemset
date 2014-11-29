#!/usr/bin/env python

import sys
from collections import Counter

def scan_file(phile):    
    with open(sys.argv[1]) as fh:
        for line in fh:
            line = line.strip()
            if len(line) == 0:
                continue
            yield [int(tok) for tok in line.split()]

# compute frequency counts
freqs = Counter()
for record in scan_file(sys.argv[1]):
    for e in record:
        freqs[e] += 1

# Create a remapping from element to frequency index
mappings = {}
for idx, (elem, cnt) in enumerate(freqs.most_common()):
    mappings[elem] = idx + 1

# Sort each record based on freqency counts
for record in scan_file(sys.argv[1]):
    srt = sorted([mappings[t] for t in record])
    print ' '.join(str(x) for x in srt)
