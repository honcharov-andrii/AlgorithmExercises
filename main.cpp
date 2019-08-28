#include "graphimpl.h"
#include "coveragecalculator.h"
#include <string>


#include <iostream>
#include <regex>
#include <string>

struct Data
{
    bool mIsRed;
};

bool checkIsRed(const Data & data)
{
    return data.mIsRed;
}

bool isSmaller(const size_t & w1, const size_t & w2)
{
    return w1 < w2;
}

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
    size_t N = 3;
    int *A = new int[N];

    for (size_t i = 0; i < N; ++i)
    {
        A[i] = rand() % 10;
    }

    //customQuickSort<int>(A, N);

    std::unordered_set<std::string> mPropertiesNeeded {"one", "two", "three", "four", "six"};
    CoverageCalculator<int, std::string> calc(mPropertiesNeeded.begin(), mPropertiesNeeded.end());

    std::unordered_set<std::string> s1 {"one"};
    calc.addObject(1, s1.begin(), s1.end());
    std::unordered_set<std::string> s2 {"two"};
    calc.addObject(2, s2.begin(), s2.end());
    std::unordered_set<std::string> s3 {"three"};
    calc.addObject(3, s3.begin(), s3.end());
    std::unordered_set<std::string> s4 {"four"};
    calc.addObject(4, s4.begin(), s4.end());
    std::unordered_set<std::string> s5 {"five"};
    calc.addObject(5, s5.begin(), s5.end());
    std::unordered_set<int> result = calc.calculateCoverage();

    /*std::string begin = "begin";
    Data d1;
    d1.mIsRed = false;

    std::string A = "A";
    Data d2;
    d2.mIsRed = false;

    std::string B = "B";
    Data d3;
    d3.mIsRed = false;

    std::string end = "end";
    Data d4;
    d4.mIsRed = true;

    IGraph<std::string, Data, size_t> *graph = new GraphImpl<std::string, Data>();
    graph->addNode(begin, std::move(d1));
    graph->addNode(A, std::move(d2));
    graph->addNode(B, std::move(d3));
    graph->addNode(end, std::move(d4));

    bool resAddEdge1 = graph->addEdge(begin, A, 6);
    bool resAddEdge2 = graph->addEdge(begin, B, 2);
    bool resAddEdge3 = graph->addEdge(B, A, 3);
    bool resAddEdge4 = graph->addEdge(A, end, 1);
    bool resAddEdge5 = graph->addEdge(B, end, 5);


    std::pair<bool, std::string> res1 =  breadthFirstSearch<std::string, Data>(*graph, begin, checkIsRed);

    std::pair<bool, std::vector<std::string>> res2 = DijkstraAlgorithm<std::string, Data, size_t>(*graph, begin, end, isSmaller);*/

    return 0;
}
