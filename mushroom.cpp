#include "fi.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using item_t = int32_t;
using BasketSetType = BasketSet<item_t>;

static void read_input(BasketSetType *input) {
    std::ifstream ifs{"../data/mushroom.dat", std::ifstream::in};

    std::string line;
    while (std::getline(ifs, line)) {
        std::vector<item_t> basket{};
        std::istringstream iss{line};
        do {
            std::string sub{};
            iss >> sub;

            if (sub.size() > 0) 
                basket.push_back(std::stoi(sub));
        } while(iss);
        input->add_basket(basket);
    }
}

int main() {
    BasketSetType input{};
    BasketSetType output{};
    read_input(&input);

    std::cout << "Read " << input.size() << " inputs." << std::endl;

    frequent_items(input, 500, &output);
    std::cout << output << std::endl;
}
