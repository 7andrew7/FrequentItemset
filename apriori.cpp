#include "apriori.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <unordered_map>


template<class T>
std::ostream& operator<<(std::ostream &out, const std::vector<T> &vec)
{
    auto begin = vec.cbegin();
    auto end = vec.cend();

    out << "(";

    if (begin != end) {
        std::cout << *begin;

        for (auto it = begin + 1; it != end; ++it) {
            out << ", " << *it;
        }
    }

    out << ")";
    return out;
}

struct BasketHash {
    std::size_t operator() (const basket_t &basket) const
    {
        std::size_t output{};
        for (item_t item : basket) {
            output ^= item;
        }

        return output;
    }
};

static inline void count_singletons(
    const basket_vector_t &baskets,
    std::size_t support,
    basket_vector_t *output)
{

    std::unordered_map<item_t, std::size_t> counts{};
    for (const basket_t &basket : baskets) {
        for (item_t item : basket) {
            counts[item]++;
        }
    }

    for (auto it = counts.cbegin(); it != counts.cend(); ++it) {
        if (it->second >= support) {
            output->push_back(basket_t{it->first});
        }
    }
}

using basket_set_iterator = basket_vector_t::const_iterator;

/**
 * Compute candidates item sets, given item sets from a previous invocation.
 */
static basket_vector_t candidate_gen(
    const basket_set_iterator prev_begin,
    const basket_set_iterator prev_end)
{
    basket_vector_t output{};
    basket_set_iterator i1{prev_begin};

    for (; i1 != prev_end; ++i1) { // for each itemset
        basket_set_iterator i2{i1 + 1};
        for (; i2 != prev_end; ++i2) {

            basket_t b1{*i1};
            basket_t b2{*i2};

            std::cout << b1 << " ; " << b2 << std::endl;

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

                output.push_back(basket);
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
    auto h_pos = haystack.cbegin();

    for (item_t item : needle) {
        for (; h_pos != haystack.cend(); ++h_pos) {
            if (*h_pos > item)
                return false;
            if (*h_pos == item) {
                ++h_pos;
                break;
            }
        }

        if (h_pos == haystack.cend())
            return false;        
    } // advance to next needle entry

    return true;
}

basket_vector_t apriori(const basket_vector_t &baskets, std::size_t support)
{
    assert(support <= baskets.size());

    ///////////////////////////////////////////////////////////////
    // Step #1: Compute frequent singletons
    basket_vector_t output{};
    count_singletons(baskets, support, &output);

    std::cout << "Singletons: " << output << std::endl;

    ///////////////////////////////////////////////////////////////
    // Step 2: iteratively produce item sets of increasing length

    // Index of the first entry in the output vector for the previous
    // iteration of apriori.
    std::size_t prev_first = 0;

    while(true) {
        auto i_begin = output.cbegin() + prev_first;
        auto i_end = output.cend();
        basket_vector_t candidates{candidate_gen(i_begin, i_end)};

        // The next iteration starts where the previous left off
        prev_first = output.size();
        std::unordered_map<basket_t, std::size_t, BasketHash> counts{};

        for (const basket_t &basket : baskets) {
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
    }

    return output;
}

int main () 
{
    basket_vector_t input = {
        {2, 5, 6},
        {2, 3, 7, 8},
        {1, 2, 3, 4, 7},
        {2, 4, 7, 9},
        {1, 2, 5, 9},
        {2, 4, 6, 7},
        {1, 2, 3, 7, 5},
        {2, 5, 6, 7}
    };

    auto result = apriori(input, 2);

    std::cout << result << std::endl;
}
