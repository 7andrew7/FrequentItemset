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
    BasketSet basket_set{
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

    const std::vector<item_t> basket{1, 6, 7};
    basket_set.add_basket(basket);

    EXPECT_EQ(9, basket_set.size());

    std::cout << basket_set << std::endl;
}

TEST(BasketSet, ForEach)
{
    using bsi = BasketSet::Basket_const_iterator;

    std::vector<std::vector<item_t>> input = {
        {2, 5, 6},
        {1, 2, 5, 9},
        {2, 3, 7, 8},
        {2, 4, 6, 7},
        {2, 4, 7, 9},
        {2, 5, 6, 7},
        {1, 2, 3, 4, 7},
        {1, 2, 3, 5, 7}
    };

    BasketSet basket_set{input};

    auto it = input.cbegin();
    std::size_t sz{0};

    basket_set.for_each([&it, &sz](bsi i1, bsi i2) {
        std::vector<item_t> vec{i1, i2};
        EXPECT_EQ(*it++, vec);
        sz++;
    });

    EXPECT_EQ(input.size(), sz);
}
