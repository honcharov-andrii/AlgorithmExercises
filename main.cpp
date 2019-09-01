#include "graphimpl.h"
#include "coveragecalculator.h"
#include "meta_programming.h"
#include <string>


template<typename T>
void customQuickSort(T *array, size_t sizeOfArray)
{
    T supportElement = array[sizeOfArray / 2];

    size_t i = 0;
    size_t j = sizeOfArray - 1;

    do
    {
        do
        {
            ++i;
        }while(array[i] <= supportElement && i < sizeOfArray);

        do
        {
            --j;
        }while(array[j] >= supportElement && j >= 0);

        if(i <= j)
        {
            T temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        }
    }while(i <= j);

    if(j > 0)
    {
        customQuickSort(array, j);
    }
    if(i < sizeOfArray)
    {
        customQuickSort(array + i, sizeOfArray - i);
    }
}

int main()
{
    int v []= {1, 2};

    CoverageCalculator<int, std::tuple<int, double, char>,
                            std::tuple<int, double>> testClass(v, std::tuple<int, double, char>(1, 1.5, 'c'),
                                                                  std::tuple<int, double>(1, 1.5));

    testClass.calculateCoverage(std::tuple<int, char>(1, 'c'));

    return 0;
}
