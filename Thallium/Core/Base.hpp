#pragma once

#include <glm/fwd.hpp>

#include <utility>

namespace Thallium {

    template<class T, class = void>
    struct IsComplete: std::false_type {};

    template<class T>
    struct IsComplete<T, decltype(void(sizeof(T)))>: std::true_type {};

#define EVAL(...)                                                   __VA_ARGS__
#define VARCOUNT(...)                                               EVAL(VARCOUNT_I(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, ))
#define VARCOUNT_I(_, _10, _9, _8, _7, _6, _5, _4, _3, _2, X_, ...) X_
#define GLUE(X, Y)                                                  GLUE_I(X, Y)
#define GLUE_I(X, Y)                                                X##Y
#define FIRST(...)                                                  EVAL(FIRST_I(__VA_ARGS__, ))
#define FIRST_I(X, ...)                                             X
#define TUPLE_TAIL(...)                                             EVAL(TUPLE_TAIL_I(__VA_ARGS__))
#define TUPLE_TAIL_I(X, ...)                                        (__VA_ARGS__)

#define TRANSFORM(NAME_, ARGS_)    (GLUE(TRANSFORM_, VARCOUNT ARGS_)(NAME_, ARGS_))
#define TRANSFORM_1(NAME_, ARGS_)  NAME_ ARGS_
#define TRANSFORM_2(NAME_, ARGS_)  NAME_(FIRST ARGS_), TRANSFORM_1(NAME_, TUPLE_TAIL ARGS_)
#define TRANSFORM_3(NAME_, ARGS_)  NAME_(FIRST ARGS_), TRANSFORM_2(NAME_, TUPLE_TAIL ARGS_)
#define TRANSFORM_4(NAME_, ARGS_)  NAME_(FIRST ARGS_), TRANSFORM_3(NAME_, TUPLE_TAIL ARGS_)
#define TRANSFORM_5(NAME_, ARGS_)  NAME_(FIRST ARGS_), TRANSFORM_4(NAME_, TUPLE_TAIL ARGS_)
#define TRANSFORM_6(NAME_, ARGS_)  NAME_(FIRST ARGS_), TRANSFORM_5(NAME_, TUPLE_TAIL ARGS_)
#define TRANSFORM_7(NAME_, ARGS_)  NAME_(FIRST ARGS_), TRANSFORM_6(NAME_, TUPLE_TAIL ARGS_)
#define TRANSFORM_8(NAME_, ARGS_)  NAME_(FIRST ARGS_), TRANSFORM_7(NAME_, TUPLE_TAIL ARGS_)
#define TRANSFORM_9(NAME_, ARGS_)  NAME_(FIRST ARGS_), TRANSFORM_8(NAME_, TUPLE_TAIL ARGS_)
#define TRANSFORM_10(NAME_, ARGS_) NAME_(FIRST ARGS_), TRANSFORM_9(NAME_, TUPLE_TAIL ARGS_)

#define defer(x)                                         \
    auto GLUE(_defer, __COUNTER__) = [&]() {             \
        auto lambda = [&]() -> void {                    \
            x;                                           \
        };                                               \
                                                         \
        class Defer {                                    \
        public:                                          \
            decltype(lambda) func;                       \
                                                         \
            Defer(decltype(lambda) func) : func(func) {} \
                                                         \
            ~Defer() {                                   \
                func();                                  \
            }                                            \
        };                                               \
                                                         \
        return Defer{ lambda };                          \
    }()

}
