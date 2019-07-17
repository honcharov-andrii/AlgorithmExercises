#ifndef IGRAPH_H
#define IGRAPH_H

#include <unordered_set>
#include <unordered_map>
#include <deque>
#include <vector>
#include <algorithm>


template<typename ID, typename DATA, typename WEIGHT = size_t>
class IGraph
{
public:
    virtual bool addNode(ID id, DATA && data) = 0;
    virtual void deleteNode(ID id) = 0;
    virtual const DATA* getDataInNode(ID id) const = 0;

    virtual std::unordered_set <ID> getChildsOfNode(ID id) const = 0;

    virtual bool addEdge(ID parent, ID child, WEIGHT weight = 0) = 0;
    virtual void deleteEdge(ID parent, ID child) = 0;
    virtual const WEIGHT* getWeight(ID parent, ID child) const = 0;

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
                    std::unordered_set<ID> childsOfNode = graph.getChildsOfNode(currentID);
                    nodesID.insert(nodesID.end(), childsOfNode.begin(), childsOfNode.end());
                }
            }
        }

        nodesID.pop_front();
    }

    return retVal;
}

template <typename ID, typename DATA, typename WEIGHT>
std::pair<bool, std::vector<ID>> DijkstraAlgorithm(const IGraph<ID, DATA> & graph, ID beginId, ID destinationID, bool (*isSmaller) (const WEIGHT & w1, const WEIGHT & w2))
{
    std::unordered_set<ID> checkedNodes;

    std::deque<ID> nodesID;

    std::unordered_map<ID, WEIGHT> minWeightToNode;

    std::unordered_map<ID, ID> childParentEdgeWithMinWeight;

    std::pair<bool, std::vector<ID>> retVal;

    retVal.first = false;

    if(nullptr == graph.getDataInNode(beginId) || nullptr == graph.getDataInNode(destinationID))
    {
        return retVal;
    }

    nodesID.push_back(beginId);

    while(false == nodesID.empty())
    {
        ID parentID = nodesID.front();

        if(checkedNodes.end() == checkedNodes.find(parentID))
        {
            checkedNodes.insert(parentID);

            std::unordered_set<ID> childsOfNode = graph.getChildsOfNode(parentID);
            nodesID.insert(nodesID.end(), childsOfNode.begin(), childsOfNode.end());

            for (auto childsOfNodeIter = childsOfNode.begin(); childsOfNodeIter != childsOfNode.end(); ++childsOfNodeIter)
            {
                const WEIGHT* weightPtr = graph.getWeight(parentID, *childsOfNodeIter);

                if(nullptr != weightPtr)
                {
                    auto childParentEdgeWithMinWeightIt = childParentEdgeWithMinWeight.find(*childsOfNodeIter);

                    if(childParentEdgeWithMinWeight.end() == childParentEdgeWithMinWeightIt)
                    {
                        minWeightToNode[*childsOfNodeIter] = *weightPtr;
                        childParentEdgeWithMinWeight[*childsOfNodeIter] = parentID;
                    }
                    else
                    {
                        if(true == isSmaller(*weightPtr, minWeightToNode[*childsOfNodeIter]))
                        {
                            minWeightToNode[*childsOfNodeIter] = *weightPtr;
                            childParentEdgeWithMinWeight[*childsOfNodeIter] = parentID;
                        }
                    }
                }
                else
                {
                    return retVal;
                }
            }
        }

        nodesID.pop_front();
    }

    retVal.second.push_back(destinationID);
    ID temp = childParentEdgeWithMinWeight.at(destinationID);

    while(true)
    {
        retVal.second.push_back(temp);

        if(temp == beginId)
        {
            break;
        }

        temp = childParentEdgeWithMinWeight.at(temp);
    }

    std::reverse(retVal.second.begin(), retVal.second.end());

    return retVal;
}

#endif // IGRAPH_H
