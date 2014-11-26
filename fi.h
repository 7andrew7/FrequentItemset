#pragma once

#include "basket_set.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <map>

/**
 * Compute frequent items for k>=2.

 * in: The immutable set of baskets
 * support: Support threshold (an integer)
 * prev_l: Frequent items from the previous generation
 * out: Final basket_set containing all frequent items.
 * current_l: Current iteration's frequent items
 */
template <class T, int k>
void frequent_items_k(
    const BasketSet<T> &in,
    typename BasketSet<T>::size_type support,
    const std::vector<std::array<T, k-1>> &prev_l,
    BasketSet<T> *out,
    std::vector<std::array<T, k>> *current_l)
{

}

/**
 * Compute frequent items over the given input set.
 */
template <class T>
void frequent_items(
    const BasketSet<T> &in,
    typename BasketSet<T>::size_type support,
    BasketSet<T> *out)
{
    using Iter = typename BasketSet<T>::Basket_const_iterator;

    // Compute frequent singletons
    std::map<T, std::size_t> C1{};
    in.for_each([&C1](Iter i1, Iter i2) {
        for (; i1 != i2; ++i1) {
            C1[*i1]++;
        }
    });

    std::vector<T> L1;
    for (auto it = C1.cbegin(); it != C1.cend(); ++it) {
        if (it->second >= support) {
            L1.push_back(it->first);
            out->add_basket(it->first);
        }
    }

    // compute doubleton candidates
    using KeyType = std::array<T, 2>;
    std::map<KeyType, std::size_t> C2{};

    for (auto it1 = L1.cbegin(); it1 != L1.cend(); ++it1) {
        for (auto it2 = it1 + 1; it2 != L1.cend(); ++it2) {
            C2[KeyType{{*it1, *it2}}] = 0;
        }
    }

    in.for_each([&C2](Iter i1, Iter i2) {
        for (auto it = C2.begin(); it != C2.end(); ++it) {
            const KeyType &key = it->first;
            if (std::includes(i1, i2, key.cbegin(), key.cend()))
                it->second++;
        }
    });

    std::vector<KeyType> L2{};
    for (auto it = C2.cbegin(); it != C2.cend(); ++it) {
        if (it->second >= support) {
            L2.push_back(it->first);
            out->add_basket(it->first);
        }
    }

    // Triple candidate generation
    using KeyType3 = std::array<T, 3>;
    std::map<KeyType3, std::size_t> C3{};
    for (auto it1 = L2.cbegin(); it1 != L2.cend(); ++it1) {
        for (auto it2 = it1 + 1; it2 != L2.cend(); ++it2) {
            const auto &b1 = *it1;
            const auto &b2 = *it2;

            auto b1_begin = b1.cbegin();
            auto b1_pre_end = b1.cend() - 1;
            auto b2_begin = b2.cbegin();
            auto b2_pre_end = b2.cend() - 1;

            if (std::equal(b1_begin, b1_pre_end, b2_begin)) {
                KeyType3 key{};
                std::copy(b1_begin, b1.cend(), key.begin());
                key[key.size() - 1] = *b2_pre_end;

                // Print in Normal order
               std::copy(key.begin(),
                         key.end(),
                         std::ostream_iterator<int>(std::cout,";")
                        );
               std::cout << std::endl;
                C3[key] = 0;
            }
        }
    }

    // Counts
    in.for_each([&C3](Iter i1, Iter i2) {
        for (auto it = C3.begin(); it != C3.end(); ++it) {
            const KeyType3 &key = it->first;
            if (std::includes(i1, i2, key.cbegin(), key.cend()))
                it->second++;
        }
    });

    // Pruning
    std::vector<KeyType3> L3{};
    for (auto it = C3.cbegin(); it != C3.cend(); ++it) {
        if (it->second >= support) {
            L3.push_back(it->first);
            out->add_basket(it->first);
        }
    }

    return;
}
