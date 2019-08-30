#include "graphimpl.h"
#include "coveragecalculator.h"
#include "meta_programming.h"
#include <string>


#include <iostream>
#include <regex>
#include <string>

template<typename ID,
         typename ...Tuples>
class   TestClass
{
private:

    std::tuple<Tuples...> tpl;
    std::vector<ID> vec;

public:
    template <size_t N>
    TestClass(ID (&arr)[N], Tuples&&... mem) :
        tpl{std::forward<Tuples>(mem)...}
    {
        static_assert(N == std::tuple_size<std::tuple<Tuples...>>::value, "Size Array of ID and tuples must be equal");

        for(auto & i : arr)
        {
            vec.push_back(i);
        }
    }

    template <typename... Args>
    std::vector<ID> calcCoverage(Args&& ... args)
    {

    }
};

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

    TestClass<int, std::tuple<int, double, char>, std::tuple<int, double>> testClass(v, std::tuple<int, double, char>(1, 1.5, 'c'), std::tuple<int, double>(1, 1.5));

    return 0;
}
