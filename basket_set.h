#pragma once

#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

/**
 * Encode a basket set as a vector containing concatenated, null-terminated
 * baskets.  Each basket is a run of items in sorted order.
 */

template <typename I, typename Container = std::vector<I>>
class BasketSet {

public:
    using size_type = std::size_t;

    using Basket_iterator = typename Container::iterator;
    using Basket_const_iterator = typename Container::const_iterator;

    BasketSet() : _item_vec{}, _size{0}, _null_elem{0} { ; }

    BasketSet(std::initializer_list<std::initializer_list<I>> baskets) :
        BasketSet{}
    {
        for (const auto &basket : baskets) {
            add_basket(basket);
        }
    }

    template<class T>
    BasketSet(const T &baskets) : BasketSet{}
    {
        for (const auto &basket : baskets) {
            add_basket(basket);
        }
    }

    void add_basket(std::initializer_list<I> basket)
    {
        _item_vec.insert(_item_vec.end(), basket);
        _item_vec.push_back(_null_elem);
        _size++;
    }

    template <class C>
    void add_basket(const C& container)
    {
        _item_vec.insert(_item_vec.end(), container.cbegin(), container.end());
        _item_vec.push_back(_null_elem);
        _size++;
    }

    template <class ForwardIterator>
    void add_basket(ForwardIterator i1, ForwardIterator i2)
    {
        _item_vec.insert(_item_vec.end(), i1, i2);
        _item_vec.push_back(_null_elem);
        _size++;
    }

    // Add a singleton item
    void add_basket(I item) {
        _item_vec.push_back(item);
        _item_vec.push_back(_null_elem);
        _size++;
    }

    template <class BinaryFunction>
    void for_each(BinaryFunction func) const
    {

        for (auto it1 = _item_vec.cbegin(); it1 != _item_vec.cend();) {
            auto it2 = it1 + 1;
            while (*it2 != _null_elem)
                ++it2;
            func(it1, it2);
            it1 = it2 + 1;
        }
    }

    size_type size() const {
        return _size;
    }

    bool operator==(const BasketSet<I, Container> &other) const
    {
        std::set<Container> s1{};
        this->for_each([&s1](Basket_const_iterator b1, Basket_const_iterator b2)
        {
            s1.insert({b1, b2});
        });

        std::set<Container> s2{};
        this->for_each([&s2](Basket_const_iterator b1, Basket_const_iterator b2)
        {
            s2.insert({b1, b2});
        });

       return (s1 == s2);
    }

private:
    // array of items; baskets are null-terminated
    Container _item_vec;
    size_type _size;
    const I _null_elem;
};

template<typename I, typename C>
std::ostream &operator<<(std::ostream &out, const BasketSet<I, C> &basket_set) {
    using bsi = typename BasketSet<I,C>::Basket_const_iterator;

    out << "( ";

    basket_set.for_each([&out](bsi i1, bsi i2) {
        out << "(";
        out << *i1++;
        for (; i1 != i2; ++i1) {
            out << ", " << *i1;
        }
        out << "), ";
    });

    out << ")";

    return out;
}
