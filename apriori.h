#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <vector>

using item_t = int32_t;

using basket_t = std::vector<item_t>;

using basket_set_t = std::vector<basket_t>;

using basket_set_iterator = basket_set_t::const_iterator;

namespace std
{
    template<>
    struct hash<basket_t>
    {
        std::size_t operator()(basket_t const& basket) const
        {
            std::size_t output{};
            for (item_t item : basket)
                output ^= item;
            return output;
        }
    };
}

template<class T>
std::ostream& operator<<(std::ostream &out, const std::vector<T> &vec)
{
    auto begin = vec.cbegin();
    auto end = vec.cend();

    out << "(";

    if (begin != end) {
        out << *begin;

        for (auto it = begin + 1; it != end; ++it) {
            out << ", " << *it;
        }
    }

    out << ")";
    return out;
}

template <class ForwardIterator>
static inline void count_singletons(
    const ForwardIterator first,
    const ForwardIterator last,
    std::size_t support,
    basket_set_t *output)
{

    std::unordered_map<item_t, std::size_t> counts{};
    for (auto it = first; it != last; ++it) {
        basket_t basket{*it};
        for (item_t item : basket)
            counts[item]++;
    }

    for (auto it = counts.cbegin(); it != counts.cend(); ++it) {
        if (it->second >= support) {
            output->push_back(basket_t{it->first});
        }
    }
}

/**
 * Compute candidates item sets, given item sets from a previous invocation.
 */
static basket_set_t candidate_gen(
    const basket_set_iterator prev_begin,
    const basket_set_iterator prev_end)
{
    basket_set_t output{};
    
    for (basket_set_iterator i1{prev_begin}; i1 != prev_end; ++i1) {
        for (basket_set_iterator i2{i1 + 1}; i2 != prev_end; ++i2) {

            const basket_t &b1{*i1};
            const basket_t &b2{*i2};

            auto b1_begin = b1.cbegin();
            auto b1_pre_end = b1.cend() - 1;
            auto b2_begin = b2.cbegin();
            auto b2_pre_end = b2.cend() - 1;

            if (std::equal(b1_begin, b1_pre_end, b2_begin)) {
                basket_t basket{b1_begin, b1_pre_end};

                item_t _min = std::min(*b1_pre_end, *b2_pre_end);
                basket.push_back(_min);

                item_t _max = std::max(*b1_pre_end, *b2_pre_end);
                basket.push_back(_max);

                output.push_back(std::move(basket));
            }
        }
    }

    return output;
}

/**
 * Determine whether the haystack basket contains all entries from the needle.
 *
 * Assumes baskets are sorted vectors.
 */
static inline bool contains(
    const basket_t &needle,
    const basket_t &haystack)
{
    auto h_pos = haystack.cbegin() - 1;

    for (item_t item : needle) {
        // invariant: h_pos points before the first candidate match
        ++h_pos;
        for (; h_pos != haystack.cend(); ++h_pos) {
            if (*h_pos > item)
                return false;
            if (*h_pos == item) {
                break;
            }
        }

        if (h_pos == haystack.cend())
            return false;
    } // advance to next needle entry

    return true;
}

template <class ForwardIterator>
basket_set_t apriori(
    const ForwardIterator first,
    const ForwardIterator last,
    std::size_t support)
{
    assert(support <= std::distance(first, last));

    ///////////////////////////////////////////////////////////////
    // Step #1: Compute frequent singletons
    basket_set_t output{};
    count_singletons(first, last, support, &output);

    ///////////////////////////////////////////////////////////////
    // Step 2: iteratively produce item sets of increasing length

    // Index of the first entry in the output vector for the previous
    // iteration of apriori.
    std::size_t prev_first = 0;

    for (auto k = 2; ; ++k) {
        auto i_begin = output.cbegin() + prev_first;
        auto i_end = output.cend();
        basket_set_t candidates{candidate_gen(i_begin, i_end)};

        // std::cout << "-------------- " << k << " -----------" << std::endl;
        // std::cout << "C " << candidates << std::endl;

        // The next iteration starts where the previous left off
        prev_first = output.size();
        std::unordered_map<basket_t, std::size_t> counts{};

        for (auto it = first; it != last; ++it) {
            const basket_t &basket{*it};
            for (const basket_t &candidate : candidates) {
                if (contains(candidate, basket)) {
                    counts[candidate]++;
                }
            }
        }

        for (auto it = counts.cbegin(); it != counts.cend(); ++it) {
            if (it->second >= support)
                output.push_back(it->first);
        }

        if (prev_first == output.size())
            return output; // nothing added this iteration

        // basket_set_t newly_added{output.cbegin() + prev_first, output.cend()};
        // std::cout << "A " << newly_added << std::endl;
    }

    return output;
}
