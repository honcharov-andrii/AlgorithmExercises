#ifndef IGRAPH_H
#define IGRAPH_H

#include <unordered_set>
#include <deque>


template<typename ID, typename DATA>
class IGraph
{
public:
    virtual bool addNode(ID id, DATA && data) = 0;
    virtual void deleteNode(ID id) = 0;
    virtual const DATA* getDataInNode(ID id) const = 0;

    virtual std::unordered_set <ID> getEdgesOfNode(ID id) const = 0;

    virtual bool addEdge(ID parent, ID child) = 0;
    virtual void deleteEdge(ID parent, ID child) = 0;

    virtual ~IGraph(){}
};

template <typename ID, typename DATA>
std::pair<bool, ID> breadthFirstSearch(const IGraph<ID, DATA> & graph, ID rootId, bool (*check) (const DATA & data))
{
    std::unordered_set<ID> checkedNodes;

    std::deque<ID> nodesID;

    std::pair<bool, ID> retVal;

    retVal.first = false;

    nodesID.push_back(rootId);

    while(false == nodesID.empty())
    {
        ID currentID = nodesID.front();

        if(checkedNodes.end() == checkedNodes.find(currentID))
        {
            checkedNodes.insert(currentID);

            const DATA* dataInNode = graph.getDataInNode(currentID);

            if(nullptr != dataInNode)
            {
                if(true == check(*dataInNode))
                {
                    retVal.first = true;
                    retVal.second = currentID;

                    break;
                }
                else
                {
                    std::unordered_set<ID> childNodes = graph.getEdgesOfNode(currentID);
                    nodesID.insert(nodesID.end(), childNodes.begin(), childNodes.end());
                }
            }
        }

        nodesID.pop_front();
    }

    return retVal;
}

#endif // IGRAPH_H
