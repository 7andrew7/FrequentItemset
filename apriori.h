#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <map>
#include <vector>

#include "basket_set2.h"
#include "timing.h"


// namespace std
// {
//     template<>
//     struct hash<basket_t>
//     {
//         std::size_t operator()(basket_t const& basket) const
//         {
//             std::size_t output{};
//             for (item_t item : basket)
//                 output ^= item;
//             return output;
//         }
//     };
// }


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

// /**
//  * Compute candidates item sets, given item sets from a previous invocation.
//  */
// static basket_set_t candidate_gen(
//     const basket_set_iterator prev_begin,
//     const basket_set_iterator prev_end)
// {
//     FUNCTION_TIMING;

//     basket_set_t output{};
    
//     for (basket_set_iterator i1{prev_begin}; i1 != prev_end; ++i1) {
//         for (basket_set_iterator i2{i1 + 1}; i2 != prev_end; ++i2) {
//             const basket_t &b1{*i1};
//             const basket_t &b2{*i2};

//             auto b1_begin = b1.cbegin();
//             auto b1_pre_end = b1.cend() - 1;
//             auto b2_begin = b2.cbegin();
//             auto b2_pre_end = b2.cend() - 1;

//             if (std::equal(b1_begin, b1_pre_end, b2_begin)) {
//                 basket_t basket{b1};
//                 basket.push_back(*b2_pre_end);
//                 output.push_back(std::move(basket));
//             }
//         }
//     }

//     return output;
// }

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

    // Index of the first entry in the output vector for the previous
    // iteration of apriori.
//     std::size_t prev_first = 0;

//     for (auto k = 2; ; ++k) {
//         auto i_begin = output.cbegin() + prev_first;
//         auto i_end = output.cend();
//         basket_set_t candidates{candidate_gen(i_begin, i_end)};

//         SCOPED_TIMING("counts");

//         // std::cout << "-------------- " << k << " -----------" << std::endl;
//         // std::cout << "C " << candidates.size() << std::endl;

//         // The next iteration starts where the previous left off
//         prev_first = output.size();
//         std::map<basket_t, std::size_t> counts{};

//         for (auto it = first; it != last; ++it) {
//             const basket_t &basket{*it};
//             for (const basket_t &candidate : candidates) {
//                 if (contains(candidate, basket)) {
//                     counts[candidate]++;
//                 }
//             }
//         }

//         for (auto it = counts.cbegin(); it != counts.cend(); ++it) {
//             if (it->second >= support) {
//                 output.push_back(it->first);
//             }
//         }

//         if (prev_first == output.size())
//             return output; // nothing added this iteration

// //        std::cout << "A " << newly_added << std::endl;
//     }

}
