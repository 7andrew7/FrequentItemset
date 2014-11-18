#include "apriori.h"

#include <cassert>
#include <iostream>
#include <unordered_map>

basket_vector_t apriori(const basket_vector_t &baskets, std::size_t support) {
    assert(support <= baskets.size());

    // Step #1: Compute frequent singletons

    std::unordered_map<item_t, std::size_t> counts{};
    for (const basket_t &basket : baskets) {
        for (item_t item : basket) {
            counts[item]++;
        }
    }

    basket_vector_t output{};
    for (auto it = counts.cbegin(); it != counts.cend(); ++it) {
        if (it->second >= support) {
            output.push_back(basket_t{it->first});
        }
    }

    // auto prev_begin = output.cbegin();
    // auto prev_end = output.cend();

    // while(true) {
    //     auto candidates = candidate_gen(prev_begin, prev_end);
    // }
    
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
