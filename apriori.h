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
    BasketSet *output)
{
    FUNCTION_TIMING;

    using Iter = Container::const_iterator;

    std::map<item_t, std::size_t> counts{};

    input.for_each([&counts](Iter i1, Iter i2) {
        for (; i1 != i2; ++i1) {
            counts[*i1]++;
        }
    });

    for (auto it = counts.cbegin(); it != counts.cend(); ++it) {
        if (it->second >= support)
            output->add_basket(it->first);
    }
}

static inline void count_singletons2(
    const BasketSet &input,
    std::size_t support,
    TrieNode *root)
{
    FUNCTION_TIMING;

    using Iter = Container::const_iterator;

    input.for_each([root](Iter i1, Iter i2) { // for each basket...
        for (; i1 != i2; ++i1) // for each basket member...
            root->increment_singleton_count(*i1);
    });
}

/**
 * Compute candidates item sets, given item sets from a previous invocation.
 */
static inline void candidate_gen(
    const BasketSet &input,
    std::size_t k, // previous iteration number
    const Container &prev_items,
    std::map<std::vector<item_t>, std::size_t> *candidates) 
{
    FUNCTION_TIMING;

    for (auto i1 = prev_items.cbegin(); i1 != prev_items.cend(); i1+=k) {
        for (auto i2 = i1 + k; i2 != prev_items.cend(); i2 += k) {
            auto i1_pre_end = i1 + k - 1;
            auto i2_pre_end = i2 + k - 1;

            if (std::equal(i1, i1_pre_end, i2)) {
                std::vector<item_t> key{i1, i1 + k};
                key.push_back(*i2_pre_end);
                (*candidates)[key] = 0;
            }
        }
    }
}

static inline void count_candidates(
    const BasketSet &input,
    std::map<std::vector<item_t>, std::size_t> *candidates)
{
    FUNCTION_TIMING;

    using Iter = Container::const_iterator;
    // Step 2: compute counts
    input.for_each([candidates](Iter i1, Iter i2) {
        for (auto it = candidates->begin(); it != candidates->end(); ++it) {
            const std::vector<item_t> &key = it->first;
            if (std::includes(i1, i2, key.cbegin(), key.cend()))
                it->second++;
        }
    });
}

static inline std::size_t select_candidates(
    const BasketSet &input,
    std::size_t support,
    const std::map<std::vector<item_t>, std::size_t> &candidates,
    Container *items)
{
    FUNCTION_TIMING;

    std::size_t count{0};

    for (auto it = candidates.cbegin(); it != candidates.cend(); ++it) {
        if (it->second >= support) {
            items->insert(items->end(), it->first.cbegin(), it->first.cend());
            count++;
        }
    }

    return count;
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

    count_singletons(input, support, output); // AAA

    count_singletons2(input, support, &root);

    root.prune_candidates(support);

    root.debug_print(std::cout);

    ///////////////////////////////////////////////////////////////
    // Step 2: iteratively produce item sets of increasing length
    ///////////////////////////////////////////////////////////////

    for (auto k = 2; ; ++k) {
        std::map<std::vector<item_t>, std::size_t> candidates{}; // AAA
        const Container &prev_items = output->get_container(k - 1); // AAA
        candidate_gen(input, k - 1, prev_items, &candidates); // AAA
        count_candidates(input, &candidates); // AAA

        root.candidate_gen(k);
        root.debug_print(std::cout);

        Container &items = output->get_container(k);
        std::size_t count = select_candidates(input, support, candidates, &items);

        if (count == 0)
            break;
    }
}
