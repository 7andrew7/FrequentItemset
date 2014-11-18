#include "apriori.h"

#include "gtest/gtest.h"

TEST(sample_test_case, sample_test)
{
    EXPECT_EQ(1, 1);
}

TEST(Apriori, BasicTest) {
    basket_vector_t input = {
        {2, 5, 6},
        {2, 3, 7, 8},
        {1, 2, 3, 4, 7},
        {2, 4, 7, 9},
        {1, 2, 5, 9},
        {2, 4, 6, 7},
        {1, 2, 3, 5, 7},
        {2, 5, 6, 7}
    };

    basket_vector_t result{apriori(input, 2)};
}
