#include "apriori.h"

#include <cassert>
#include <iostream>
#include <unordered_map>


static inline void count_singletons(
    const basket_vector_t &baskets,
    std::size_t support,
    basket_vector_t *output) {

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

using basket_iter_t = basket_vector_t::const_iterator;

static inline basket_vector_t candidate_gen(
    basket_iter_t prev_begin, basket_iter_t prev_end) {
    basket_vector_t candidates{};
    return candidates;
}

static inline bool contains(const basket_t &needle, const basket_t &haystack) {
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

struct BasketHash {
    std::size_t operator() (const basket_t &basket) const {
        std::size_t output{};
        for (item_t item : basket) {
            output ^= item;
        }

        return output;
    }
};

basket_vector_t apriori(const basket_vector_t &baskets, std::size_t support) {
    assert(support <= baskets.size());

    // Step #1: Compute frequent singletons
    basket_vector_t output{};
    count_singletons(baskets, support, &output);

    auto prev_begin = output.cbegin();
    auto prev_end = output.cend();

    while(true) {
        basket_vector_t candidates{candidate_gen(prev_begin, prev_end)};
        prev_begin = prev_end;

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

        prev_end = output.cend();
        if (prev_begin == prev_end)
            return output;
    }

    return output;
}

template<class T>
std::ostream& operator<<(std::ostream &out, const std::vector<T> &vec) {
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

int main () {
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
