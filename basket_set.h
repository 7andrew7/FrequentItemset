#pragma once

#include <algorithm>
#include <iostream>
#include <vector>

/**
 * Encode a basket set as a vector containing concatenated, null-terminated
 * baskets.  Each basket is a run of items in sorted order.
 */
using item_t = int32_t;

static constexpr item_t null_elem = 0;

class BasketSet {

public:
    using size_type = std::size_t;

    using Container = std::vector<item_t>;
    using Basket_iterator = Container::iterator;
    using Basket_const_iterator = Container::const_iterator;

    BasketSet() : _item_vec{}, _size{0} { ; }

    BasketSet(std::initializer_list<std::initializer_list<item_t>> baskets) :
        _item_vec{}, _size{0}
    {
        for (const auto &basket : baskets) {
            add_basket(basket);
        }
    }

    void add_basket(std::initializer_list<item_t> basket)
    {
        _item_vec.insert(_item_vec.end(), basket);
        _item_vec.push_back(null_elem);
        _size++;

    }

    template <class C>
    void add_basket(const C& container)
    {
        _item_vec.insert(_item_vec.end(), container.cbegin(), container.end());
        _item_vec.push_back(null_elem);
        _size++;
    }

    void add_basket(Basket_const_iterator i1, Basket_const_iterator i2)
    {
        _item_vec.insert(_item_vec.end(), i1, i2);
        _item_vec.push_back(null_elem);
        _size++;
    }

    template <class BinaryFunction>
    void for_each(BinaryFunction func) const
    {
        auto it1 = _item_vec.cbegin();

        for (; it1 != _item_vec.cend(); ++it1) {
            for (auto it2 = it1 + 1; it2 != _item_vec.cend() && *it2 != 0; ++it2) {
                func(it1, it2);
                it1 = it2 + 1;
            }
        }
    }

    size_type size() const {
        return _size;
    }

private:
    // array of items; baskets are null-terminated
    Container _item_vec;
    size_type _size;
};

std::ostream &operator<<(std::ostream &out, const BasketSet &basket_set) {
    using bsi = BasketSet::Basket_const_iterator;

    out << "( ";

    basket_set.for_each([&out](bsi i1, bsi i2) {
        out << "( ";
        for (; i1 != i2; ++i1) {
            out << *i1 << " ";
        }
        out << ")";
    });

    return out;
}
