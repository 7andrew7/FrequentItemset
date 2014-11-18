#include <cstdint>
#include <vector>

using item_t = int32_t;

using basket_t = std::vector<item_t>;

using basket_vector_t = std::vector<basket_t>;

namespace std
{
    template<>
    struct hash<basket_t>
    {
        std::size_t operator()(basket_t const& basket) const
        {
            std::size_t output{};
            for (item_t item : basket)
                output ^= item;
            return output;
        }
    };
}

basket_vector_t apriori(const basket_vector_t &, std::size_t);
