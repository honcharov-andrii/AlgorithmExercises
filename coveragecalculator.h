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

namespace check_is_optional_templates
{


    template<typename TypeListOne,
             template <typename...> class FirstTuple>
    struct OptionalTypes
    {

    };


}

namespace two_tuples_for_each
{
    template <typename... >
    struct Typelist {};

    template<int index,
             typename TypeListOne,
             typename TypeListTwo,
             template <typename...> class FirstTuple,
             template <typename...> class SecondTuple,
             typename Callback>
    struct iterate_tuple;

    template<int index,
             typename... FirstTupleArgs,
             typename... SecondTupleArgs,
             template <typename...> class FirstTuple,
             template <typename...> class SecondTuple,
             typename Callback>
    struct iterate_tuple<index,
            Typelist <FirstTupleArgs...>,
            Typelist <SecondTupleArgs...>,
            FirstTuple,
            SecondTuple,
            Callback>
    {
        static void next(const FirstTuple<FirstTupleArgs...>& firstTuple, const SecondTuple<SecondTupleArgs...>& secondTuple, std::vector<bool> & matchingValues, Callback callback)
        {
            iterate_tuple<index - 1,
                    Typelist <FirstTupleArgs...>,
                    Typelist <SecondTupleArgs...>,
                    FirstTuple,
                    SecondTuple, Callback>::next(firstTuple, secondTuple, matchingValues, callback);

            callback(index, std::get<index>(firstTuple), secondTuple, matchingValues);
        }
    };

    template<typename... FirstTupleArgs,
             typename... SecondTupleArgs,
             template <typename...> class FirstTuple,
             template <typename...> class SecondTuple,
             typename Callback>
    struct iterate_tuple<0,
            Typelist <FirstTupleArgs...>,
            Typelist <SecondTupleArgs...>,
            FirstTuple,
            SecondTuple,
            Callback>
    {
        static void next(const FirstTuple<FirstTupleArgs...>& firstTuple, const SecondTuple<SecondTupleArgs...>& secondTuple, std::vector<bool> & matchingValues, Callback callback)
        {
            callback(0, std::get<0>(firstTuple), secondTuple, matchingValues);
        }
    };

    template<typename... FirstTupleArgs,
             typename... SecondTupleArgs,
             template <typename...> class FirstTuple,
             template <typename...> class SecondTuple,
             typename Callback>
    struct iterate_tuple<-1,
            Typelist <FirstTupleArgs...>,
            Typelist <SecondTupleArgs...>,
            FirstTuple,
            SecondTuple,
            Callback>
    {
        static void next(const FirstTuple<FirstTupleArgs...>& firstTuple, const SecondTuple<SecondTupleArgs...>& secondTuple, std::vector<bool> & matchingValues, Callback callback){ }
    };

    template <typename... FirstTupleArgs, template <typename...> class FirstTuple,
              typename... SecondTupleArgs, template <typename...> class SecondTuple,
              typename Callback>
    void for_each(const FirstTuple<FirstTupleArgs...> & firstTuple, const SecondTuple<SecondTupleArgs...> & secondTuple, std::vector<bool> & matchingValues, Callback callback)
    {
        int const t_size = std::tuple_size<std::tuple<FirstTupleArgs...>>::value;

        iterate_tuple<t_size - 1,
                Typelist <FirstTupleArgs...>,
                Typelist <SecondTupleArgs...>,
                FirstTuple,
                SecondTuple, Callback>::next(firstTuple, secondTuple, matchingValues, callback);
    }
};

template <size_t N, typename ID, typename ...Properties>
class CoverageCalculator
{
private:
    std::unordered_map<ID, std::tuple<Properties...>> mStorage;
    std::unordered_map<ID, std::array<bool, N>> mValidProperties;
    std::tuple<Properties...> mPropertiesNeeded;

public:
    CoverageCalculator(Properties&& ...props) :
        mStorage{},
        mPropertiesNeeded{std::forward<Properties...>(props)...}
    {}

    template <typename ...OptionProperties>
    void addObject(const ID & id, OptionProperties&& ...optionProperties)
    {
        unique_template_types::UniqueTypes<Properties...> checkIsUnique;

        std::tuple<OptionProperties...> optProps(std::forward<OptionProperties...>(optionProperties)...);



        //mStorage[id] = std::unordered_set<Props>(begin, end);
    }

    bool eraseObject(const ID & id)
    {
        size_t tmp = mStorage.erase(id); // std::size_type ??????
        mValidProperties.erase(id);

        return tmp > 0 ? true : false;
    }

    void clear()
    {
        mStorage.clear();
        mValidProperties.clear();
    }

    std::unordered_set<ID> calculateCoverage()
    {
        std::unordered_set<ID> retVal;

        /*std::unordered_set<Props> tmpPropertiesNeeded = mPropertiesNeeded;

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
        }*/

        return retVal;
    }
};

#endif // COVERAGECALCULATOR_H
