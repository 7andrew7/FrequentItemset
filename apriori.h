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
static void candidate_gen(
    const BasketSet &input,
    std::size_t support,
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

    for (auto k = 1; ; ++k) {
        std::map<std::vector<item_t>, std::size_t> candidates{};
        const Container &prev_items = output->get_container(k);
        candidate_gen(input, support, k, prev_items, &candidates);
        // basket_set_t candidates{candidate_gen(i_begin, i_end)};

        // SCOPED_TIMING("counts");

        // // std::cout << "-------------- " << k << " -----------" << std::endl;
        // // std::cout << "C " << candidates.size() << std::endl;

        // // The next iteration starts where the previous left off
        // prev_first = output.size();
        // std::map<basket_t, std::size_t> counts{};

        // for (auto it = first; it != last; ++it) {
        //     const basket_t &basket{*it};
        //     for (const basket_t &candidate : candidates) {
        //         if (contains(candidate, basket)) {
        //             counts[candidate]++;
        //         }
        //     }
        // }

        // for (auto it = counts.cbegin(); it != counts.cend(); ++it) {
        //     if (it->second >= support) {
        //         output.push_back(it->first);
        //     }
        // }

        // if (prev_first == output.size())
        //     return output; // nothing added this iteration

//        std::cout << "A " << newly_added << std::endl;
//     }
    }
}
