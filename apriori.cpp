#include "apriori.h"

#include <cassert>
#include <iostream>

basket_vector_t apriori(const basket_vector_t &baskets, std::size_t support) {
    assert(support <= baskets.size());

    basket_vector_t output{};

    return baskets;
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
        {2, 5, 5},
        {2, 3, 7, 8},
        {1, 2, 3, 4, 7},
        {2, 4, 5, 7},
        {1, 2, 5},
        {2, 4, 6, 7},
        {1, 2, 3, 7, 5},
        {2, 5, 6, 7}
    };

    auto result = apriori(input, 2);

    std::cout << result << std::endl;
}
