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
        void operator()(int index, const T & t, const U & u)
        {
            int a = 0;
        }
    };

    struct CallbackForIterateTuple
    {
        template<typename T, typename... Arg>
        void operator()(int index, const T & t, const std::tuple<Arg...>& tpl)
        {
            tuple_for_each::for_each_with_param(tpl, CallbackForCompareValues(), t);
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

        std::vector<bool> conrolResult(std::tuple_size<std::tuple<Args...>>::value);

        for(size_t i = 0; i < mStorage.size(); ++i)
        {
            ID bestId;

            for(auto it = mStorage.begin(); it != mStorage.end(); ++it)
            {
                if(retVal.end() == retVal.find(it->first))
                {
                    std::vector<bool> localConrolResult = conrolResult;

                    tuple_for_each::for_each(it->second, mPropertiesNeeded, CallbackForIterateTuple());

                    if(std::count(localConrolResult.begin(), localConrolResult.end(), true) > std::count(conrolResult.begin(), conrolResult.end(), true))
                    {
                        conrolResult = localConrolResult;

                        bestId = it->first;
                    }
                }
            }

            retVal.insert(bestId);

            if(conrolResult.size() == std::count(conrolResult.begin(), conrolResult.end(), true))// all variables in vector will be true
            {
                break;
            }
        }

        return retVal;
    }
};

#endif // COVERAGECALCULATOR_H
