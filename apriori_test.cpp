#include "apriori.h"

#include <unordered_set>

#include "gtest/gtest.h"

TEST(sample_test_case, sample_test)
{
    EXPECT_EQ(1, 1);
}

/**
 * Brute force implementation of frequent itemset mining
 */
std::unordered_set<basket_t> brute_force_frequent_items(
    const basket_set_t &input,
    std::size_t support,
    std::size_t range)
{
    std::unordered_set<basket_t> output{};

    auto max_val = (1 << range);
    for (std::size_t i = 1; i < max_val; ++i) {
        basket_t candidate{};
        for (std::size_t j = 0 ; j < range; ++j) {
            if ((i>>j) & 1)
                candidate.push_back(j + 1);
        }

        std::size_t count = 0;
        for (const basket_t &basket : input) {
            if (contains(candidate, basket)) {
                if (++count == support)
                    output.insert(candidate);
            }
        }
    }

    return output;
}

TEST(Apriori, Contains) {
    EXPECT_TRUE(contains({1, 3, 5}, {0, 1, 2, 3, 4, 5}));
    EXPECT_TRUE(contains({1}, {0, 1, 2, 3, 4, 5}));
    EXPECT_FALSE(contains({1, 7}, {0, 1, 2, 3, 4, 5}));
    EXPECT_FALSE(contains({1, 3}, {0, 1, 2, 4, 5}));
}

TEST(Apriori, BasicTest) {

    const basket_set_t input {
        {2, 5, 6},
        {1, 2, 5, 9},
        {2, 3, 7, 8},
        {2, 4, 6, 7},
        {2, 4, 7, 9},
        {2, 5, 6, 7},
        {1, 2, 3, 4, 7},
        {1, 2, 3, 5, 7}
    };

    const basket_set_t result{apriori(input.cbegin(), input.cend(), 2)};
    const std::unordered_set<basket_t> result_set{result.cbegin(), result.cend()};

    const auto expected_set = brute_force_frequent_items(input, 2, 9);
    EXPECT_EQ(expected_set, result_set);
}
