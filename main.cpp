#include "graphimpl.h"

struct Data
{
    bool mIsRed;
};

bool checkIsRed(const Data & data)
{
    return data.mIsRed;
}

int main()
{
    int id1 = 1;
    Data d1;
    d1.mIsRed = false;

    int id2 = 2;
    Data d2;
    d2.mIsRed = true;

    IGraph<int, Data> *graph = new GraphImpl<int, Data>();
    graph->addNode(id1, std::move(d1));
    graph->addNode(id2, std::move(d2));

    bool resAddEdge1 = graph->addEdge(id1, id2);

    graph->deleteEdge(id1, id2);

    graph->addEdge(id1, id2);

    std::pair<bool, int> res =  breadthFirstSearch<int, Data>(*graph, id1, checkIsRed);

    return 0;
}
