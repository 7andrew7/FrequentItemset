#include "apriori.h"

#include <iostream>

basket_vector_t apriori(const basket_vector_t &baskets, std::size_t support) {
    basket_vector_t output{baskets};
    return output;
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

    for (const auto &basket : result) {
        for (auto item : basket) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }
}
