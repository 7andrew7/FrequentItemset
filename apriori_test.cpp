#include "apriori.h"

#include <unordered_set>

#include "gtest/gtest.h"

TEST(sample_test_case, sample_test)
{
    EXPECT_EQ(1, 1);
}

TEST(Apriori, BasicTest) {

    const std::unordered_set<basket_t> input {
        {2, 5, 6},
        {2, 3, 7, 8},
        {1, 2, 3, 4, 7},
        {2, 4, 7, 9},
        {1, 2, 5, 9},
        {2, 4, 6, 7},
        {1, 2, 3, 5, 7},
        {2, 5, 6, 7}
    };

    basket_vector_t result{apriori(input.cbegin(), input.cend(), 2)};
    const std::unordered_set<basket_t> result_set{result.cbegin(), result.cend()};

    const std::unordered_set<basket_t> expected_set
    {
        {1},
        {2},
        {3},
        {4},
        {5},
        {6},
        {7},
        {9},
        {4,7},
        {1,3},
        {5,6},
        {3,7},
        {2,5},
        {1,2},
        {2,9},
        {1,5},
        {2,6},
        {2,3},
        {6,7},
        {2,7},
        {5,7},
        {1,7},
        {2,4},
        {1,3,7},
        {2,4,7},
        {1,2,7},
        {2,6,7},
        {2,5,7},
        {1,2,3},
        {2,5,6},
        {2,3,7},
        {1,2,5},
        {1,2,3,7},
    };

    EXPECT_EQ(result_set, expected_set);
}
