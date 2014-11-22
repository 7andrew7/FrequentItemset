#include "basket_set.h"

#include <iostream>

#include "gtest/gtest.h"

TEST(BasketSet, DefaultConstructor)
{
    BasketSet basket_set{};

    EXPECT_EQ(0, basket_set.size());

    basket_set.add_basket({2, 5, 6});
    basket_set.add_basket({1, 3, 7});

    EXPECT_EQ(2, basket_set.size());
}

TEST(BasketSet, InitializerListConstructor)
{
    const BasketSet basket_set{
        {2, 5, 6},
        {1, 2, 5, 9},
        {2, 3, 7, 8},
        {2, 4, 6, 7},
        {2, 4, 7, 9},
        {2, 5, 6, 7},
        {1, 2, 3, 4, 7},
        {1, 2, 3, 5, 7}
    };

    EXPECT_EQ(8, basket_set.size());
}
