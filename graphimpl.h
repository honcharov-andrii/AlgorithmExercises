#ifndef GRAPHIMPL_H
#define GRAPHIMPL_H

#include <unordered_map>
#include <map>

#include "igraph.h"

template<typename ID, typename DATA, typename WEIGHT = size_t>
class GraphImpl : public IGraph<ID, DATA, WEIGHT>
{
private:
    std::unordered_multimap<ID, ID> mEdgesFromNode;
    std::unordered_multimap<ID, ID> mEdgesToNode;
    std::unordered_map<ID, DATA> mNodes;
    std::map<std::pair<ID, ID>, WEIGHT> mWeights;

public:

    GraphImpl() = default;

    virtual ~GraphImpl() = default;

    virtual bool addNode(ID id, DATA && data) override
    {
        bool retVal = false;

        if(mNodes.end() == mNodes.find(id))
        {
            mNodes[id] = std::forward<DATA>(data);

            retVal = true;
        }

        return retVal;
    }

    virtual void deleteNode(ID id) override
    {
        auto resultFromNode = mEdgesFromNode.equal_range(id);

        for (auto it = resultFromNode.first; it != resultFromNode.second; ++it)
        {
            mWeights.erase(std::pair<ID, ID>(it->first, it->second));
        }

        mEdgesFromNode.erase(id);
        mEdgesToNode.erase(id);
        mNodes.erase(id);
    }

    virtual const DATA* getDataInNode(ID id) const override
    {
        const DATA* retVal = nullptr;

        auto it = mNodes.find(id);

        if(mNodes.end() != it)
        {
            retVal = &(it->second);
        }

        return retVal;
    }

    virtual std::unordered_set<ID> getChildsOfNode(ID id) const override
    {
        std::unordered_set<ID> retVal;

        auto result = mEdgesFromNode.equal_range(id);

        for (auto it = result.first; it != result.second; ++it)
        {
            retVal.insert(it->second);
        }

        return retVal;
    }

    virtual bool addEdge(ID parent, ID child, WEIGHT weight = 0) override
    {
        bool retVal = false;

        if(mNodes.end() != mNodes.find(parent) &&
           mNodes.end() != mNodes.find(child))
        {
            mEdgesFromNode.insert(std::pair<ID, ID> (parent, child));
            mEdgesToNode.insert(std::pair<ID, ID> (child, parent));
            mWeights.insert(std::pair<std::pair<ID, ID>, WEIGHT>(std::pair<ID, ID>(parent, child), weight));//?????????

            retVal = true;
        }

        return retVal;
    }

    virtual void deleteEdge(ID parent, ID child) override
    {
        auto resultFromNode = mEdgesFromNode.equal_range(parent);

        for (auto it = resultFromNode.first; it != resultFromNode.second; ++it)
        {
            if(child == (*it).second)
            {
                mEdgesFromNode.erase(it);
                break;
            }
        }

        auto resultToNode = mEdgesToNode.equal_range(child);

        for (auto it = resultToNode.first; it != resultToNode.second; ++it)
        {
            if(parent == (*it).second)
            {
                mEdgesToNode.erase(it);
                break;
            }
        }

        auto resultWeightIt = mWeights.find(std::pair<ID, ID> (parent, child));

        if(mWeights.end() != resultWeightIt)
        {
            mWeights.erase(resultWeightIt);
        }
    }

    virtual const WEIGHT* getWeight(ID parent, ID child) const override
    {
        const WEIGHT* retVal = nullptr;

        auto it = mWeights.find(std::pair<ID, ID>(parent, child));

        if(mWeights.end() != it)
        {
            retVal = &(it->second);
        }

        return retVal;
    }
};

#endif // GRAPHIMPL_H
