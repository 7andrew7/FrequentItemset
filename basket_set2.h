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

    // Add a singleton basket
    void add_basket(item_t item)
    {
        _containers[1].push_back(item);
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

    template <class BinaryFunction>
    void for_each(BinaryFunction func) const
    {
        for (std::size_t sz = 1; sz < MAX_BASKET_SIZE; ++sz) {
            const Container &container = _containers[sz];
            for (auto it = container.cbegin(); it != container.cend(); it += sz) {
                func(it, it + sz);
            }
        }
    }

    Container &get_container(std::size_t sz)
    {
        return _containers[sz];
    }

    const Container &get_container(std::size_t sz) const
    {
        return _containers[sz];
    }

private:
    std::array<Container, MAX_BASKET_SIZE> _containers;
};

std::ostream &operator<<(std::ostream &out, const BasketSet &basket_set) {

    std::ostream_iterator<item_t> out_it (out, ",");
    using Iter = Container::const_iterator;
    basket_set.for_each([&out, &out_it](Iter i1, Iter i2) {
        out << "(";
        std::copy(i1, i2, out_it);
        out << "),";
    });

    return out;
}
