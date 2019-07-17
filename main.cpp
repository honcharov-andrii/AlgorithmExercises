#include "graphimpl.h"
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

int main()
{
    std::string begin = "begin";
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

    std::pair<bool, std::vector<std::string>> res2 = DijkstraAlgorithm<std::string, Data, size_t>(*graph, begin, end, isSmaller);

    return 0;
}
