#pragma once

#include "basket_set.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <map>

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
            std::cout << *i1 << " " << C1[*i1] << std::endl;
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

    // in.for_each([&C2](Iter i1, Iter i2) {

    //     if (std::includes(i1, i2, ))
    //         counts[*i1]++;
    //         std::cout << *i1 << " " << counts[*i1] << std::endl;
    //     }
    // });

    return;
}
