#pragma once

#include "basket_set.h"

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
    std::map<T, std::size_t> counts{};
    in.for_each([&counts](Iter i1, Iter i2) {
        for (; i1 != i2; ++i1) {
            counts[*i1]++;
            std::cout << *i1 << " " << counts[*i1] << std::endl;
        }
    });

    for (auto it = counts.cbegin(); it != counts.cend(); ++it) {
        if (it->second >= support) {
            out->add_basket(it->first);
        }
    }

    std::cout << *out << std::endl;
}
