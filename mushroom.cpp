#include "apriori.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

static void read_input(BasketSet *input) {
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
    BasketSet input{};
    BasketSet output{};
    read_input(&input);

    std::cout << "Read " << input.size() << " inputs." << std::endl;

    apriori(input, 500, &output);
    std::cout << output << std::endl;
}
