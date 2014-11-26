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
template <class T, unsigned long k>
void frequent_items_k(
    const BasketSet<T> &in,
    typename BasketSet<T>::size_type support,
    const std::vector<std::array<T, k-1>> &prev_l,
    BasketSet<T> *out,
    std::vector<std::array<T, k>> *current_l)
{
    using KeyType = std::array<T, k>;
    using PrevKeyType = std::array<T, k-1>;

    using Iter = typename BasketSet<T>::Basket_const_iterator;

    // Step 1: generate candidates
    std::map<KeyType, std::size_t> candidate_counts{};

    for (auto it1 = prev_l.cbegin(); it1 != prev_l.cend(); ++it1) {
        for (auto it2 = it1 + 1; it2 != prev_l.cend(); ++it2) {
            const PrevKeyType &b1 = *it1;
            const PrevKeyType &b2 = *it2;

            auto b1_begin = b1.cbegin();
            auto b1_pre_end = b1.cend() - 1;
            auto b2_begin = b2.cbegin();
            auto b2_pre_end = b2.cend() - 1;

            if (std::equal(b1_begin, b1_pre_end, b2_begin)) {
                KeyType key{};
                std::copy(b1_begin, b1.cend(), key.begin());
                key[key.size() - 1] = *b2_pre_end;

                candidate_counts[key] = 0;
            }
        }
    }

    // Step 2: compute counts
    in.for_each([&candidate_counts](Iter i1, Iter i2) {
        for (auto it = candidate_counts.begin(); it != candidate_counts.end(); ++it) {
            const KeyType &key = it->first;
            if (std::includes(i1, i2, key.cbegin(), key.cend()))
                it->second++;
        }
    });

    // Step 3: Select candidates with support
    for (auto it = candidate_counts.cbegin(); it != candidate_counts.cend(); ++it) {
        if (it->second >= support) {
            current_l->push_back(it->first);
            out->add_basket(it->first);
        }
    }
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
    std::vector<std::array<T, 3>> L3{};
    frequent_items_k(in, support, L2, out, &L3);

    // k=4
    std::vector<std::array<T, 4>> L4{};
    frequent_items_k(in, support, L3, out, &L4);

    return;
}
