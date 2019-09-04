#ifndef COVERAGECALCULATOR_H
#define COVERAGECALCULATOR_H

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <experimental/optional>
#include <tuple>

namespace unique_template_types
{
    template <typename... Types>
    struct UniqueTypes;

    template <typename T1, typename T2, typename ... Types>
    struct UniqueTypes<T1, T2, Types ...>
     : UniqueTypes<T1, T2>, UniqueTypes<T1, Types ...>, UniqueTypes<T2, Types ...> {};

    template <typename T, typename U>
    struct UniqueTypes<T, U>
    {
        static_assert(!std::is_same<T, U>::value, "Types must be unique");
    };
}

namespace two_tuples_for_each
{
    template <typename... >
    struct Typelist {};

    template<size_t N,
             int index,
             typename TypeListOne,
             typename TypeListTwo,
             template <typename...> class FirstTuple,
             template <typename...> class SecondTuple,
             typename Callback>
    struct iterate_tuple;

    template<size_t N,
             int index,
             typename... FirstTupleArgs,
             typename... SecondTupleArgs,
             template <typename...> class FirstTuple,
             template <typename...> class SecondTuple,
             typename Callback>
    struct iterate_tuple<N,
            index,
            Typelist <FirstTupleArgs...>,
            Typelist <SecondTupleArgs...>,
            FirstTuple,
            SecondTuple,
            Callback>
    {
        static void next(const FirstTuple<FirstTupleArgs...>& firstTuple, SecondTuple<SecondTupleArgs...>& secondTuple, std::array<bool, N> & validValues, Callback callback)
        {
            iterate_tuple<N,
                    index - 1,
                    Typelist <FirstTupleArgs...>,
                    Typelist <SecondTupleArgs...>,
                    FirstTuple,
                    SecondTuple, Callback>::next(firstTuple, secondTuple, validValues, callback);

            callback(index, std::get<index>(firstTuple), secondTuple, validValues);
        }
    };

    template<size_t N,
             typename... FirstTupleArgs,
             typename... SecondTupleArgs,
             template <typename...> class FirstTuple,
             template <typename...> class SecondTuple,
             typename Callback>
    struct iterate_tuple<N,
            0,
            Typelist <FirstTupleArgs...>,
            Typelist <SecondTupleArgs...>,
            FirstTuple,
            SecondTuple,
            Callback>
    {
        static void next(const FirstTuple<FirstTupleArgs...>& firstTuple, SecondTuple<SecondTupleArgs...>& secondTuple, std::array<bool, N> & validValues, Callback callback)
        {
            callback(0, std::get<0>(firstTuple), secondTuple, validValues);
        }
    };

    template<size_t N,
             typename... FirstTupleArgs,
             typename... SecondTupleArgs,
             template <typename...> class FirstTuple,
             template <typename...> class SecondTuple,
             typename Callback>
    struct iterate_tuple<N,
            -1,
            Typelist <FirstTupleArgs...>,
            Typelist <SecondTupleArgs...>,
            FirstTuple,
            SecondTuple,
            Callback>
    {
        static void next(const FirstTuple<FirstTupleArgs...>& firstTuple, SecondTuple<SecondTupleArgs...>& secondTuple, std::array<bool, N> & validValues, Callback callback){ }
    };

    template <size_t N,
              typename... FirstTupleArgs, template <typename...> class FirstTuple,
              typename... SecondTupleArgs, template <typename...> class SecondTuple,
              typename Callback>
    void for_each_for_two_tuples(const FirstTuple<FirstTupleArgs...> & firstTuple, SecondTuple<SecondTupleArgs...> & secondTuple, std::array<bool, N> & validValues, Callback callback)
    {
        int const t_size = std::tuple_size<std::tuple<FirstTupleArgs...>>::value;

        iterate_tuple<N, t_size - 1,
                Typelist <FirstTupleArgs...>,
                Typelist <SecondTupleArgs...>,
                FirstTuple,
                SecondTuple, Callback>::next(firstTuple, secondTuple, validValues, callback);
    }
};

namespace for_each_tuple_with_param
{
    template<size_t N, int index, typename Callback, typename Param, typename... Args>
    struct iterate_tuple_with_param
    {
        static void next(std::tuple<Args...> & tpl, const Param & param, std::array<bool, N> & validValues, Callback callback)
        {
            iterate_tuple_with_param<N, index - 1, Callback, Param, Args...>::next(tpl, param, validValues, callback);

            callback(index, std::get<index>(tpl), param, validValues);
        }
    };

    template<size_t N, typename Callback, typename Param, typename... Args>
    struct iterate_tuple_with_param<N, 0, Callback, Param, Args...>
    {
        static void next(std::tuple<Args...> & tpl, const Param & param, std::array<bool, N> & validValues, Callback callback)
        {
            callback(0, std::get<0>(tpl), param, validValues);
        }
    };

    template<size_t N, typename Callback, typename Param, typename... Args>
    struct iterate_tuple_with_param<N, -1, Callback, Param, Args...>
    {
        static void next(std::tuple<Args...> & tpl, const Param & param, std::array<bool, N> & validValues, Callback callback){}
    };

    template<size_t N, typename Callback, typename Param, typename... Args>
    void for_each_with_param(std::tuple<Args...> & tpl, const Param & param, std::array<bool, N> & validValues, Callback callback)
    {
        int const t_size = std::tuple_size<std::tuple<Args...>>::value;

        iterate_tuple_with_param<N, t_size - 1, Callback, Param, Args...>::next(tpl, param, validValues, callback);
    }
}

namespace compare_two_tuples
{
    template<size_t N, int index, typename Callback, typename ...Args>
    struct iterate_tuple
    {
        static void next(const std::tuple<Args...>& firstTuple, const std::tuple<Args...>& secondTuple, const std::array<bool, N> & validValues, std::array<bool, N> & matchingValues, Callback callback)
        {
            iterate_tuple<N, index - 1, Callback, Args...>::next(firstTuple, secondTuple, validValues, matchingValues, callback);

            callback(index, std::get<index>(firstTuple), std::get<index>(secondTuple), validValues, matchingValues);
        }
    };

    template<size_t N, typename Callback, typename ...Args>
    struct iterate_tuple<N, 0, Callback, Args...>
    {
        static void next(const std::tuple<Args...>& firstTuple, const std::tuple<Args...>& secondTuple, const std::array<bool, N> & validValues, std::array<bool, N> & matchingValues, Callback callback)
        {
            callback(0, std::get<0>(firstTuple), std::get<0>(secondTuple), validValues, matchingValues);
        }
    };

    template<size_t N, typename Callback, typename ...Args>
    struct iterate_tuple<N, -1, Callback, Args...>
    {
        static void next(const std::tuple<Args...>& firstTuple, const std::tuple<Args...>& secondTuple, const std::array<bool, N> & validValues, std::array<bool, N> & matchingValues, Callback callback){ }
    };

    template <size_t N, typename Callback, typename ...Args>
    void for_each_for_compare_two_tuples(const std::tuple<Args...> & firstTuple, const std::tuple<Args...> & secondTuple, const std::array<bool, N> & validValues, std::array<bool, N> & matchingValues, Callback callback)
    {
        int const t_size = std::tuple_size<std::tuple<Args...>>::value;

        iterate_tuple<N, t_size - 1, Callback, Args...>::next(firstTuple, secondTuple, validValues, matchingValues, callback);
    }
};


template <size_t N, typename ID, typename ...Properties>
class CoverageCalculator
{
private:
    std::unordered_map<ID, std::tuple<Properties...>> mStorage;
    std::unordered_map<ID, std::array<bool, N>> mValidProperties;
    std::tuple<Properties...> mPropertiesNeeded;

    struct CallbackForCompareVariablesOnTypes
    {
        template<typename T, typename U>
        void operator()(int index, T & tupleParam, const U & inputParam, std::array<bool, N> & validValues)
        {}

        template<typename T>
        void operator()(int index, T & tupleParam, const T & inputParam, std::array<bool, N> & validValues)
        {
            tupleParam = inputParam;
            validValues[index] = true;
        }
    };

    struct CallbackForIterateTuple
    {
        template<typename T, typename... Arg>
        void operator()(int index, const T & t, std::tuple<Arg...> & tpl, std::array<bool, N> & validValues)
        {
            for_each_tuple_with_param::for_each_with_param(tpl, t, validValues, CallbackForCompareVariablesOnTypes());
        }
    };

    struct CallbackForCompareValues
    {
        template<typename T>
        void operator()(int index, const T & t1, const T & t2, const std::array<bool, N> & validValues, std::array<bool, N> & matchingValues)
        {
            if(true == validValues[index])
            {
                matchingValues[index] = t1 == t2 ? true : false;
            }
        }
    };

public:
    CoverageCalculator(Properties&& ...props) :
        mStorage{},
        mPropertiesNeeded{std::forward<Properties>(props)...}
    {
        unique_template_types::UniqueTypes<Properties...> checkIsUnique;
    }

    template <typename ...InputProperties>
    void addObject(const ID & id, InputProperties&& ...inputProperties)
    {
        unique_template_types::UniqueTypes<InputProperties...> checkIsUnique;

        std::tuple<InputProperties...> inpProps(std::forward<InputProperties>(inputProperties)...);

        std::tuple<Properties...> castedProperties;

        std::array<bool, N> validValues{};

        two_tuples_for_each::for_each_for_two_tuples(inpProps, castedProperties, validValues, CallbackForIterateTuple());

        mStorage[id] = std::move(castedProperties);

        mValidProperties[id] = std::move(validValues);
    }

    void eraseObject(const ID & id)
    {
        mStorage.erase(id);
        mValidProperties.erase(id);
    }

    void clear()
    {
        mStorage.clear();
        mValidProperties.clear();
    }

    std::unordered_set<ID> calculateCoverage()
    {
        std::unordered_set<ID> retVal;

        std::array<bool, N> bestResult{};

        for(size_t i = 0; i < mStorage.size(); ++i)
        {
            std::pair<bool, ID> bestId;
            bestId.first = false;

            std::array<bool, N> bestResultAfterCycle{};

            auto storageIt = mStorage.begin();
            auto validPropIt = mValidProperties.begin();

            while(storageIt != mStorage.end() && validPropIt != mValidProperties.end())
            {
                if(retVal.end() == retVal.find(storageIt->first))
                {
                    std::array<bool, N> localBestResult = bestResult;

                    compare_two_tuples::for_each_for_compare_two_tuples(storageIt->second, mPropertiesNeeded, validPropIt->second, localBestResult,
                                                                                                                                   CallbackForCompareValues());

                    if(std::count(localBestResult.begin(), localBestResult.end(), true) >
                       std::count(bestResultAfterCycle.begin(), bestResultAfterCycle.end(), true))
                    {
                        bestResultAfterCycle = localBestResult;

                        bestId.first = true;
                        bestId.second = storageIt->first;
                    }
                }

                ++storageIt;
                ++validPropIt;
            }

            if(true == bestId.first)
            {
                retVal.insert(bestId.second);
                bestResult = bestResultAfterCycle;
            }

            if(bestResult.size() == std::count(bestResult.begin(), bestResult.end(), true))// all variables in vector will be true
            {
                break;
            }
        }

        return retVal;
    }
};

#endif // COVERAGECALCULATOR_H
