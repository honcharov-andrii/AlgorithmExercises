#ifndef COVERAGECALCULATOR_H
#define COVERAGECALCULATOR_H

#include <vector>
#include <unordered_set>
#include <algorithm>
#include <tuple>
#include "meta_programming.h"

template <typename ID, typename ...Tuples>
class CoverageCalculator
{
private:
    std::tuple<Tuples...> mTupleOfTuples;
    std::vector<ID> mIDs;

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
        void operator()(int index, const T & t, const std::tuple<Arg...> & tpl, std::vector<bool> & matchingValues)
        {
            tuple_for_each::for_each_with_param(tpl, t, matchingValues, CallbackForCompareValues());
        }
    };

    struct CallbackForTupleOfTuples
    {
        template<typename... FirstTupleArgs, template <typename...> class FirstTuple,
                 typename... SecondTupleArgs, template <typename...> class SecondTuple>
        void operator()(int index, const FirstTuple<FirstTupleArgs...> & firstTuple, const SecondTuple<SecondTupleArgs...> & secondTuple, std::vector<bool> & matchingValues)
        {
            int a = 10;
        }
    };

public:
    template <size_t N>
    CoverageCalculator(ID (&arr)[N], Tuples&&... tuples) :
        mTupleOfTuples{std::forward<Tuples>(tuples)...}
    {
        static_assert(N == std::tuple_size<std::tuple<Tuples...>>::value, "Size Array of ID and tuples must be equal");

        for(auto & i : arr)
        {
            mIDs.push_back(i);
        }
    }

    template <typename... Args>
    std::unordered_set<ID> calculateCoverage(Args&&... args)
    {
        std::unordered_set<ID> retVal;

        std::tuple<Args...> propertiesNeeded(std::forward<Args>(args)...);

        std::vector<bool> bestResult(std::tuple_size<std::tuple<Args...>>::value);

        for(size_t i = 0; i < std::tuple_size<std::tuple<Tuples...>>::value; ++i)
        {
            ID bestId;

            std::vector<bool> bestResultAfterCycle = bestResult;

            for_each_for_tuple_of_tuples::for_each_tuple_of_tuples(
                                              mTupleOfTuples,
                                              propertiesNeeded,
                                              bestResult,
                                              mIDs,
                                              retVal,
                                              bestId,
                                              bestResultAfterCycle,
                                              CallbackForTupleOfTuples());

            /*for(auto it = mStorage.begin(); it != mStorage.end(); ++it)
            {
                if(retVal.end() == retVal.find(it->first))
                {
                    std::vector<bool> localBestResult = bestResult;

                    tuple_for_each::for_each(mTupleOfTuples, propertiesNeeded, localBestResult, CallbackForTupleOfTuples());

                    if(std::count(localBestResult.begin(), localBestResult.end(), true) > std::count(bestResultAfterCycle.begin(), bestResultAfterCycle.end(), true))
                    {
                        bestResultAfterCycle = localBestResult;

                        bestId = it->first;
                    }
                }
            }*/

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
