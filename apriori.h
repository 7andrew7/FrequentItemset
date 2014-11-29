#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <map>
#include <vector>

#include "trie.h"
#include "basket_set2.h"
#include "timing.h"

static inline void count_singletons(
    const BasketSet &input,
    std::size_t support,
    TrieNode *root)
{
    using Iter = Container::const_iterator;

    input.for_each([root](Iter i1, Iter i2) { // for each basket...
        for (; i1 != i2; ++i1) // for each basket member...
            root->increment_singleton_count(*i1);
    });
}

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

    ///////////////////////////////////////////////////////////////
    // Step #1: Compute frequent singletons
    ///////////////////////////////////////////////////////////////

    BEGIN_TIMING("singleton")

    count_singletons(input, support, &root);
    root.prune_candidates(support, 1);

    END_TIMING;

    ///////////////////////////////////////////////////////////////
    // Step 2: iteratively produce item sets of increasing length
    ///////////////////////////////////////////////////////////////

    for (auto k = 2; ; ++k) {
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
