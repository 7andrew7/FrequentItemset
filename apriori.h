#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <map>
#include <vector>

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
    std::size_t k,
    const std::map<std::vector<item_t>, std::size_t> &candidates,
    BasketSet *output)
{
    Container items = output->get_container(k);
    std::size_t count{0};

    for (auto it = candidates.cbegin(); it != candidates.cend(); ++it) {
        if (it->second >= support) {
            items.insert(items.end(), it->first.cbegin(), it->first.cend());
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

    ///////////////////////////////////////////////////////////////
    // Step #1: Compute frequent singletons
    ///////////////////////////////////////////////////////////////

    count_singletons(input, support, output);

    ///////////////////////////////////////////////////////////////
    // Step 2: iteratively produce item sets of increasing length
    ///////////////////////////////////////////////////////////////

    for (auto k = 2; ; ++k) {
        std::map<std::vector<item_t>, std::size_t> candidates{};
        const Container &prev_items = output->get_container(k - 1);

        candidate_gen(input, k - 1, prev_items, &candidates);
        count_candidates(input, &candidates);
        std::size_t count = select_candidates(input, support, k, candidates, output);

        if (count == 0)
            break;
    }
}
