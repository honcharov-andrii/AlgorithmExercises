#ifndef COVERAGECALCULATOR_H
#define COVERAGECALCULATOR_H

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <tuple>
#include "meta_programming.h"

template <typename ID, typename ...Args>
class CoverageCalculator
{
private:
    std::unordered_map<ID, std::tuple<Args...>> mStorage;
    std::tuple<Args...> mPropertiesNeeded;

    struct CallbackForCompareValues
    {
        template<typename T, typename U>
        void operator()(int index, const T & t, const U & u, std::vector<bool> & matchingValues)
        {}

        template<typename T>
        void operator()(int index, const T & t1, const T & t2, std::vector<bool> & matchingValues)
        {
            if(t1 == t2)
            {
                matchingValues[index] = true;
            }
        }
    };

    struct CallbackForIterateTuple
    {
        template<typename T, typename... Arg>
        void operator()(int index, const T & t, const std::tuple<Arg...>& tpl, std::vector<bool> & matchingValues)
        {
            tuple_for_each::for_each_with_param(tpl, t, matchingValues, CallbackForCompareValues());
        }
    };

public:
    CoverageCalculator(Args&&... args) :
        mStorage{},
        mPropertiesNeeded{std::forward<Args>(args)...}
    {
        unique_template_types::UniqueTypes<Args...> checkIsUnique;
    }

    void addObject(const ID & id, Args&&... args)
    {
        mStorage[id] = std::make_tuple(std::forward<Args>(args)...);
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

        std::vector<bool> bestResult(std::tuple_size<std::tuple<Args...>>::value);

        for(size_t i = 0; i < mStorage.size(); ++i)
        {
            ID bestId;

            std::vector<bool> bestResultAfterCycle = bestResult;

            for(auto it = mStorage.begin(); it != mStorage.end(); ++it)
            {
                if(retVal.end() == retVal.find(it->first))
                {
                    std::vector<bool> localBestResult = bestResult;

                    tuple_for_each::for_each(it->second, mPropertiesNeeded, localBestResult, CallbackForIterateTuple());

                    if(std::count(localBestResult.begin(), localBestResult.end(), true) > std::count(bestResultAfterCycle.begin(), bestResultAfterCycle.end(), true))
                    {
                        bestResultAfterCycle = localBestResult;

                        bestId = it->first;
                    }
                }
            }

            retVal.insert(bestId);

            bestResult = bestResultAfterCycle;

            if(bestResult.size() == std::count(bestResult.begin(), bestResult.end(), true))// all variables in vector will be true
            {
                break;
            }
        }

        return retVal;
    }
};

#endif // COVERAGECALCULATOR_H
