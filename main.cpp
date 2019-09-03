#include "coveragecalculator.h"
#include <string>


int main()
{
    CoverageCalculator<3, std::string, int, char, bool> test(1, 'c', false);

    test.addObject("red", 1, true);
    //test.addObject("green", 'c', false);
    test.addObject("blue", 'a', 33, 64.5);

    std::unordered_set<std::string> res = test.calculateCoverage();

    return 0;
}
