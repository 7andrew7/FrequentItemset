#include <algorithm>
#include <unordered_set>

#include "gtest/gtest.h"

#include "apriori.h"
#include "basket_set2.h"


TEST(sample_test_case, sample_test)
{
    EXPECT_EQ(1, 1);
}

/**
 * Brute force implementation of frequent itemset mining
 */
void brute_force_frequent_items(
    const BasketSet &input,
    std::size_t support,
    std::size_t range,
    BasketSet *output)
{
    using Iter = Container::const_iterator;

    std::size_t max_val = (1 << range);
    for (std::size_t i = 1; i < max_val; ++i) {
        std::vector<item_t> candidate{};
        std::size_t count{0};

        for (std::size_t j = 0 ; j < range; ++j) {
            if ((i>>j) & 1)
                candidate.push_back(j + 1);
        }

        input.for_each([&candidate, &count](Iter i1 , Iter i2) {
            if (std::includes(i1, i2, candidate.cbegin(), candidate.cend()))
                count++;
        });
        if (count >= support)
            output->add_basket(candidate);
    }
}

TEST(Apriori, BasicTest) {

    const BasketSet input {
        {2, 5, 6},
        {1, 2, 5, 9},
        {2, 3, 7, 8},
        {2, 4, 6, 7},
        {2, 4, 7, 9},
        {2, 5, 6, 7},
        {1, 2, 3, 4, 7},
        {1, 2, 3, 5, 7}
    };

    BasketSet expected{};
    brute_force_frequent_items(input, 2, 9, &expected);

    std::cout << expected << std::endl;

    // const basket_set_t result{apriori(input.cbegin(), input.cend(), 2)};
    // const std::unordered_set<basket_t> result_set{result.cbegin(), result.cend()};

    // const auto expected_set = brute_force_frequent_items(input, 2, 9);
    // EXPECT_EQ(expected_set, result_set);
}
