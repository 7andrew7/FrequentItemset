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
    using size_t = std::size_t;

public:
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

    size_t size() const {
        return _size;
    }

private:
    // array of items; baskets are null-terminated
    Container _item_vec;
    size_t _size;
};

std::ostream &operator<<(std::ostream &out, const BasketSet &basket_set) {
    out << "Print something here!";
    return out;
}
