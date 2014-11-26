#pragma once

#include <array>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>

using item_t = int32_t;
using Container = std::vector<item_t>;

static constexpr int32_t MAX_BASKET_SIZE = 32;

/**
 * Encode a basket set as a list of vectors of increasing length.
 */

class BasketSet {

public:
    BasketSet() : _containers{} {};

    BasketSet(std::initializer_list<std::initializer_list<item_t>> baskets) :
        BasketSet{}
    {
        for (const auto &basket : baskets) {
            add_basket(basket);
        }
    }

    template<class InputIterator>
    void add_basket(InputIterator i1, InputIterator i2)
    {
        auto sz = std::distance(i1, i2);
        _containers[sz].insert(_containers[sz].end(), i1, i2);
    }

    template <class C>
    void add_basket(const C& container)
    {
        add_basket(container.cbegin(), container.cend());
    }

    void add_basket(std::initializer_list<item_t> ls)
    {
        add_basket(ls.begin(), ls.end());
    }

    std::size_t size() const 
    {
        std::size_t count{};
        for (std::size_t sz = 1; sz < MAX_BASKET_SIZE; ++sz) {
            count += _containers[sz].size() / sz;
        }
        return count;
    }

    bool operator==(const BasketSet &other) const
    {
       return (_containers == other._containers);
    }

    friend std::ostream & operator<<(std::ostream &, const BasketSet&);

private:
    std::array<Container, MAX_BASKET_SIZE> _containers;
};

std::ostream &operator<<(std::ostream &out, const BasketSet &basket_set) {

    std::ostream_iterator<item_t> out_it (out, ",");

    for (std::size_t sz = 1; sz < MAX_BASKET_SIZE; ++sz) {
        const Container &container = basket_set._containers[sz];
        for (auto it = container.cbegin(); it != container.cend(); it += sz) {
            out << "(";
            std::copy(it, it + sz, out_it);
            out << "),";
        }
    }
    return out;
}
