#include "basket_set.h"

#include <iostream>
#include <sstream>

#include "gtest/gtest.h"

TEST(BasketSet, DefaultConstructor)
{
    BasketSet<int32_t> basket_set{};

    EXPECT_EQ(0, basket_set.size());

    basket_set.add_basket({2, 5, 6});
    basket_set.add_basket({1, 3, 7});

    EXPECT_EQ(2, basket_set.size());
    EXPECT_EQ(basket_set, basket_set);
}

TEST(BasketSet, Equality)
{
    BasketSet<int32_t> b1{{2, 5, 6},{1, 2, 5, 9}};
    BasketSet<int32_t> b2{{1, 2, 5, 9}, {2, 5, 6}};

    EXPECT_EQ(b1, b2);
}

TEST(BasketSet, InitializerListConstructor)
{
    BasketSet<int32_t> basket_set{
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

    const std::vector<int32_t> basket{1, 6, 7};
    basket_set.add_basket(basket);

    EXPECT_EQ(9, basket_set.size());
}

TEST(BasketSet, StreamOut)
{
    BasketSet<int32_t> basket_set{{2, 5, 6}, {1, 2, 5, 9}};

    std::stringstream ss;
    ss << basket_set;

    ASSERT_EQ("( (2, 5, 6), (1, 2, 5, 9), )", ss.str());
}

TEST(BasketSet, ForEach)
{
    std::vector<std::vector<int16_t>> input = {
        {2, 5, 6},
        {1, 2, 5, 9},
        {2, 3, 7, 8},
        {2, 4, 6, 7},
        {2, 4, 7, 9},
        {2, 5, 6, 7},
        {1, 2, 3, 4, 7},
        {1, 2, 3, 5, 7}
    };

    BasketSet<int16_t> basket_set{input};

    auto it = input.cbegin();
    std::size_t sz{0};

    // work around lack of generic lambda in c++11
    using Iter = decltype(basket_set)::Basket_const_iterator;

    basket_set.for_each([&it, &sz](Iter i1, Iter i2) {
        std::vector<int16_t> vec{i1, i2};
        EXPECT_EQ(*it++, vec);
        sz++;
    });

    EXPECT_EQ(input.size(), sz);
}
