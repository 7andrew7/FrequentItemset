#include "apriori.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

static void read_input(std::vector<basket_t> *input) {
    std::ifstream ifs{"../data/mushroom.dat", std::ifstream::in};

    std::string line;
    while (std::getline(ifs, line)) {
        basket_t basket{};
        std::istringstream iss{line};
        do {
            std::string sub{};
            iss >> sub;

            if (sub.size() > 0) 
                basket.push_back(std::stoi(sub));
        } while(iss);
        input->push_back(std::move(basket));
    }
}

int main() {
    std::vector<basket_t> input;
    read_input(&input);

    std::cout << "Read " << input.size() << " inputs." << std::endl;

    auto output = apriori(input.cbegin(), input.cend(), 500);
    std::cout << output << std::endl;
}
