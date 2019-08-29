#ifndef META_PROGRAMMING_H
#define META_PROGRAMMING_H

#include <type_traits>
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
    template < typename... >
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
        static void next(FirstTuple<FirstTupleArgs...>& firstTuple, SecondTuple<SecondTupleArgs...>& secondTuple, Callback callback)
        {
            iterate_tuple<index - 1,
                    Typelist <FirstTupleArgs...>,
                    Typelist <SecondTupleArgs...>,
                    FirstTuple,
                    SecondTuple, Callback>::next(firstTuple, secondTuple, callback);

            callback(index, std::get<index>(firstTuple), secondTuple);
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
        static void next(FirstTuple<FirstTupleArgs...>& firstTuple, SecondTuple<SecondTupleArgs...>& secondTuple, Callback callback)
        {
            callback(0, std::get<0>(firstTuple), secondTuple);
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
        static void next(FirstTuple<FirstTupleArgs...>& firstTuple, SecondTuple<SecondTupleArgs...>& secondTuple, Callback callback){ }
    };

    template <typename... FirstTupleArgs, template <typename...> class FirstTuple,
              typename... SecondTupleArgs, template <typename...> class SecondTuple,
              typename Callback>
    void for_each(FirstTuple<FirstTupleArgs...>& firstTuple, SecondTuple<SecondTupleArgs...>& secondTuple, Callback callback)
    {
        int const t_size = std::tuple_size<std::tuple<FirstTupleArgs...>>::value;

        iterate_tuple<t_size - 1,
                Typelist <FirstTupleArgs...>,
                Typelist <SecondTupleArgs...>,
                FirstTuple,
                SecondTuple, Callback>::next(firstTuple, secondTuple, callback);
    }




    template<int index,
             typename Callback,
             typename T,
             typename... Args>
    struct iterate_tuple_with_param
    {
        static void next(std::tuple<Args...>& tpl, Callback callback, T& param)
        {
            iterate_tuple_with_param<index - 1,
                                     Callback,
                                     T,
                                     Args...>::next(tpl, callback, param);

            callback(index, std::get<index>(tpl), param);
        }
    };

    template<typename Callback, typename T, typename... Args>
    struct iterate_tuple_with_param<0, Callback, T, Args...>
    {
        static void next(std::tuple<Args...>& tpl, Callback callback, T& param)
        {
            callback(0, std::get<0>(tpl), param);
        }
    };

    template<typename Callback, typename T, typename... Args>
    struct iterate_tuple_with_param<-1, Callback, T, Args...>
    {
        static void next(std::tuple<Args...>& tpl, Callback callback, T& param){ }
    };

    template<typename Callback, typename T, typename... Args>
    void for_each_with_param(std::tuple<Args...>& tpl, Callback callback, T& param)
    {
        int const t_size = std::tuple_size<std::tuple<Args...>>::value;

        iterate_tuple_with_param<t_size - 1, Callback, T, Args...>::next(tpl, callback, param);
    }
}

#endif // META_PROGRAMMING_H
