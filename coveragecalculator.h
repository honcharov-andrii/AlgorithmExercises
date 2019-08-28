#ifndef COVERAGECALCULATOR_H
#define COVERAGECALCULATOR_H

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

template <typename ID, typename Props>
class CoverageCalculator
{
private:
    std::unordered_map<ID, std::unordered_set<Props>> mStorage;
    std::unordered_set<Props> mPropertiesNeeded;

public:
    template <typename Iterator>
    CoverageCalculator(const Iterator & begin, Iterator end) :
        mStorage{},
        mPropertiesNeeded{begin, end}
    {}

    template <typename Iterator>
    void addObject(const ID & id, const Iterator & begin, const Iterator & end)
    {
        mStorage[id] = std::unordered_set<Props>(begin, end);
    }

    bool eraseObject(const ID & id)
    {
        size_t tmp = mStorage.erase(id); // std::size_type ??????

        return tmp > 0 ? true : false;
    }

    void clear()
    {
        mStorage.clear();
    }

    std::unordered_set<ID> calculateCoverage()
    {
        std::unordered_set<ID> retVal;

        std::unordered_set<Props> tmpPropertiesNeeded = mPropertiesNeeded;

        for(size_t i = 0; i < mStorage.size() && tmpPropertiesNeeded.size() > 0; ++i)
        {
            ID bestId;
            std::unordered_set<Props> bestIntersection;

            for(auto it = mStorage.begin(); it != mStorage.end(); ++it)
            {
                std::unordered_set<Props> currentIntersection;

                std::set_intersection(tmpPropertiesNeeded.begin(), tmpPropertiesNeeded.end(),
                                      it->second.begin(), it->second.end(),
                                      std::inserter(currentIntersection, currentIntersection.begin()));

                if(currentIntersection.size() > bestIntersection.size())
                {
                    bestId = it->first;
                    bestIntersection = currentIntersection;
                }
            }

            retVal.insert(bestId);

            std::unordered_set<Props> diff;

            std::set_difference(tmpPropertiesNeeded.begin(), tmpPropertiesNeeded.end(),
                                bestIntersection.begin(), bestIntersection.end(),
                                std::inserter(diff, diff.begin()));

            tmpPropertiesNeeded = diff;
        }

        return retVal;
    }
};

#endif // COVERAGECALCULATOR_H
