#ifndef REFLECT_H
#define REFLECT_H

#include <array>
#include <string>
#include <tuple>
#include <functional>
#include <type_traits>

//David Mazières implementation of for each
/************************************************************************/
#define PARENS ()

#define EXPAND(...) EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))
#define EXPAND4(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND3(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__

// The FOR_EACH macro recursively applies a macro to each argument in __VA_ARGS__
#define FOR_EACH(macro, ...)                                    \
  __VA_OPT__(EXPAND(FOR_EACH_HELPER(macro, __VA_ARGS__)))
#define FOR_EACH_HELPER(macro, a1, ...)                         \
  macro(a1)                                                     \
  __VA_OPT__(FOR_EACH_AGAIN PARENS (macro, __VA_ARGS__))
#define FOR_EACH_AGAIN() FOR_EACH_HELPER

/************************************************************************/
// Count the number of arguments (this helps with reflection of members)
#define COUNT_ARGS_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, count, ...) count
#define COUNT_ARGS(...) COUNT_ARGS_IMPL(__VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

// Simple stringification for each member
#define TOSTRING(ARG) #ARG,// extra , to seperate each elements in the array

//Creats class to string
#define CLASSTOSTRING(CLASS) \
    inline static constexpr auto classname(){\
        return #CLASS;\
    }

// This is where reflection happens for each member
#define REFLECT_MEMBER(CLASSNAME, ...)\
    inline auto membercount(){\
        return  COUNT_ARGS(__VA_ARGS__);\
    }\
    inline auto member(){\
        return std::tie(__VA_ARGS__);\
    }\
    inline static const auto Names() {\
        return std::array<std::string, COUNT_ARGS(__VA_ARGS__)> {FOR_EACH(TOSTRING, __VA_ARGS__)};\
    }\
    template <typename T>\
    void ApplyFunction(T&& function){\
        std::tuple members = member();\
        std::apply(\
        [&function]<typename... ClassTypes>(ClassTypes&... _args){((function(_args)), ...);}\
        , members);\
    }

// The macro to reflect all members in the struct/class
#define REFLECTABLE(CLASSNAME, ...) \
    CLASSTOSTRING(CLASS)\
    __VA_OPT__(REFLECT_MEMBER(CLASSNAME, __VA_ARGS__))


#endif