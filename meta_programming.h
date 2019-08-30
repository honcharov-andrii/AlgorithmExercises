#ifndef META_PROGRAMMING_H
#define META_PROGRAMMING_H

#include <type_traits>
#include <vector>
#include <tuple>

namespace unique_template_types
{
    template <typename... Types>
    struct UniqueTypes;

    template <class T1, class T2, class ... Types>
    struct UniqueTypes<T1, T2, Types ...>
     : UniqueTypes<T1, T2>, UniqueTypes<T1, Types ...>, UniqueTypes<T2, Types ...> {};

    template <class T, class U>
    struct UniqueTypes<T, U>
    {
        static_assert(!std::is_same<T, U>::value, "Types must be unique");
    };
}

namespace tuple_for_each
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


    template<int index, typename Callback, typename Param, typename... Args>
    struct iterate_tuple_with_param
    {
        static void next(const std::tuple<Args...> & tpl, const Param & param, std::vector<bool> & matchingValues, Callback callback)
        {
            iterate_tuple_with_param<index - 1, Callback, Param, Args...>::next(tpl, param, matchingValues, callback);

            callback(index, std::get<index>(tpl), param, matchingValues);
        }
    };

    template<typename Callback, typename Param, typename... Args>
    struct iterate_tuple_with_param<0, Callback, Param, Args...>
    {
        static void next(const std::tuple<Args...> & tpl, const Param & param, std::vector<bool> & matchingValues, Callback callback)
        {
            callback(0, std::get<0>(tpl), param, matchingValues);
        }
    };

    template<typename Callback, typename Param, typename... Args>
    struct iterate_tuple_with_param<-1, Callback, Param, Args...>
    {
        static void next(const std::tuple<Args...> & tpl, const Param & param, std::vector<bool> & matchingValues, Callback callback){}
    };

    template<typename Callback, typename Param, typename... Args>
    void for_each_with_param(const std::tuple<Args...> & tpl, const Param & param, std::vector<bool> & matchingValues, Callback callback)
    {
        int const t_size = std::tuple_size<std::tuple<Args...>>::value;

        iterate_tuple_with_param<t_size - 1, Callback, Param, Args...>::next(tpl, param, matchingValues, callback);
    }
}

#endif // META_PROGRAMMING_H
