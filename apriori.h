#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <map>
#include <vector>

#include "trie.h"
#include "basket_set.h"
#include "timing.h"

static inline void count_candidates(
    const BasketSet &input,
    std::size_t size,
    TrieNode *root)
{
    using Iter = Container::const_iterator;

    input.for_each([size, root](Iter i1, Iter i2) { // for each basket...
        root->increment_combinations(i1, i2, size);
    });
}

void apriori(
    const BasketSet &input,
    std::size_t support,
    BasketSet *output)
{
    TrieNode root{};

    for (auto k = 1; ; ++k) {
        // Iteratively produce item sets of increasing length
        SCOPED_TIMING("loop");
        count_candidates(input, k, &root);
        auto remaining = root.prune_candidates(support, k);

        if (remaining == 0)
            break;
    }

    // Unpack trie results into the output data structure.
    root.export_output(output);
    return;
}
