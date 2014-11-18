#include <cstdint>
#include <vector>

using item_t = int32_t;

using basket_t = std::vector<item_t>;

using basket_vector_t = std::vector<basket_t>;

basket_vector_t apriori(const basket_vector_t &, std::size_t);
