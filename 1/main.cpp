#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

#include "simplerng.hpp"
#include "integerlcg.hpp"

int main()
{
    SimpleRNG generator(3, 0.9, 1);

    generator.reset(0.4);
    std::vector<double> vec;
    std::copy(generator.begin(), generator.end(0.05), std::back_inserter(vec));

    std::cout << "vector: ";
    std::for_each(vec.begin(), vec.end(), [](double elem)
                  { std::cout << elem << " "; });

    std::cout << "\n";
    generator.reset(0);
    for (auto x : generator)
        std::cout << x << " ";

    std::cout << "\ncube: ";
    IntegerLCG<> gen{};
    std::uniform_int_distribution<int> dist_int(1, 6);
    for (int i = 0; i < 10; i++)
        std::cout << dist_int(gen) << " ";

    std::cout << "\ndouble_distribution : ";
    std::uniform_real_distribution<double> dist_real(0.0, 1.0);
    for (int i = 0; i < 10; i++)
        std::cout << dist_real(gen) << " ";

    std::cout << "\nshuffle vector: ";
    std::vector<int> v = {1, 2, 3, 4, 5, 6};
    std::shuffle(v.begin(), v.end(), gen);
    std::for_each(v.begin(), v.end(), [](int elem)
                  { std::cout << elem << " "; });
}