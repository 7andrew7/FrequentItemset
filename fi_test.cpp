#include "fi.h"

#include <iostream>

#include "gtest/gtest.h"

TEST(FrequentItems, EmptyBaskets)
{
    const BasketSet<int32_t> input{};
    BasketSet<int32_t> output{};

    frequent_items(input, 0, &output);
    EXPECT_EQ(input, output);
    EXPECT_EQ(0, output.size());
}

TEST(FrequentItems, SmallBasket)
{
    const BasketSet<int32_t> input{
        {2, 5, 6},
        {1, 2, 5, 9},
        {2, 3, 7, 8},
        {2, 4, 6, 7},
        {2, 4, 7, 9},
        {2, 5, 6, 7},
        {1, 2, 3, 4, 7},
        {1, 2, 3, 5, 7}
    };

    BasketSet<int32_t> output{};

    frequent_items(input, 2, &output);

    std::cout << output << std::endl;
}
