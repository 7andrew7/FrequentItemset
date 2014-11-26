#include "basket_set2.h"

#include <iostream>
#include <sstream>

#include "gtest/gtest.h"

TEST(BasketSet, DefaultConstructor)
{
    BasketSet basket_set{};

    EXPECT_EQ(0, basket_set.size());

    basket_set.add_basket({2, 5, 6});
    basket_set.add_basket({1, 3, 7});

    EXPECT_EQ(2, basket_set.size());
    EXPECT_EQ(basket_set, basket_set);

    std::cout << basket_set << std::endl;
}
