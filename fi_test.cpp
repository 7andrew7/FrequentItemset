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
