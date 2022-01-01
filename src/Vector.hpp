#pragma once

#include <array>

template<size_t N, typename T>
struct Vector {
    std::array<T, N> Elements;

    constexpr T& operator[](size_t index) {
        return Elements[index];
    }

    constexpr const T& operator[](size_t index) const {
        return Elements[index];
    }
};

#define OPERATOR(op)                                                                          \
    template<size_t N, typename T>                                                            \
    inline constexpr Vector<N, T> operator op(const Vector<N, T>& a, const Vector<N, T>& b) { \
        Vector<N, T> result;                                                                  \
        for (size_t i = 0; i < N; i++) {                                                      \
            result[i] = a[i] op b[i];                                                         \
        }                                                                                     \
        return result;                                                                        \
    }                                                                                         \
                                                                                              \
    template<size_t N, typename T>                                                            \
    inline constexpr Vector<N, T> operator op(const Vector<N, T>& a, const T& b) {            \
        Vector<N, T> result;                                                                  \
        for (size_t i = 0; i < N; i++) {                                                      \
            result[i] = a[i] op b;                                                            \
        }                                                                                     \
        return result;                                                                        \
    }                                                                                         \
                                                                                              \
    template<size_t N, typename T>                                                            \
    inline constexpr Vector<N, T> operator op(const T& a, const Vector<N, T>& b) {            \
        Vector<N, T> result;                                                                  \
        for (size_t i = 0; i < N; i++) {                                                      \
            result[i] = a op b[i];                                                            \
        }                                                                                     \
        return result;                                                                        \
    }

OPERATOR(+)
OPERATOR(-)
OPERATOR(*)
OPERATOR(/)
OPERATOR(%)
OPERATOR(&)
OPERATOR(|)
OPERATOR(^)
OPERATOR(<<)
OPERATOR(>>)

#undef OPERATOR

#define OPERATOR_EQUALS(op)                                                              \
    template<size_t N, typename T>                                                       \
    inline constexpr Vector<N, T>& operator op(Vector<N, T>& a, const Vector<N, T>& b) { \
        for (size_t i = 0; i < N; i++) {                                                 \
            a[i] op b[i];                                                                \
        }                                                                                \
        return a;                                                                        \
    }                                                                                    \
                                                                                         \
    template<size_t N, typename T>                                                       \
    inline constexpr Vector<N, T>& operator op(Vector<N, T>& a, const T& b) {            \
        for (size_t i = 0; i < N; i++) {                                                 \
            a[i] op b;                                                                   \
        }                                                                                \
        return a;                                                                        \
    }

OPERATOR_EQUALS(+=)
OPERATOR_EQUALS(-=)
OPERATOR_EQUALS(*=)
OPERATOR_EQUALS(/=)
OPERATOR_EQUALS(%=)
OPERATOR_EQUALS(&=)
OPERATOR_EQUALS(|=)
OPERATOR_EQUALS(^=)
OPERATOR_EQUALS(<<=)
OPERATOR_EQUALS(>>=)

#undef OPERATOR_EQUALS

template<typename T>
struct Vector<1, T> {
    union {
        std::array<T, 1> Elements;
        struct {
            T x;
        };
        struct {
            T r;
        };
    };

    constexpr T& operator[](size_t index) {
        return Elements[index];
    }

    constexpr const T& operator[](size_t index) const {
        return Elements[index];
    }
};

template<typename T>
using Vector2  = Vector<2, T>;
using Vector2f = Vector2<float>;

template<typename T>
struct Vector<2, T> {
    union {
        std::array<T, 2> Elements;
        struct {
            T x;
            T y;
        };
        struct {
            T r;
            T g;
        };
    };

    constexpr T& operator[](size_t index) {
        return Elements[index];
    }

    constexpr const T& operator[](size_t index) const {
        return Elements[index];
    }
};

template<typename T>
using Vector2  = Vector<2, T>;
using Vector2f = Vector2<float>;

template<typename T>
struct Vector<3, T> {
    union {
        std::array<T, 3> Elements;
        struct {
            T x;
            T y;
            T z;
        };
        struct {
            T r;
            T g;
            T b;
        };
    };

    constexpr T& operator[](size_t index) {
        return Elements[index];
    }

    constexpr const T& operator[](size_t index) const {
        return Elements[index];
    }
};

template<typename T>
using Vector3  = Vector<3, T>;
using Vector3f = Vector3<float>;

template<typename T>
struct Vector<4, T> {
    union {
        std::array<T, 4> Elements;
        struct {
            T x;
            T y;
            T z;
            T w;
        };
        struct {
            T r;
            T g;
            T b;
            T a;
        };
    };

    constexpr T& operator[](size_t index) {
        return Elements[index];
    }

    constexpr const T& operator[](size_t index) const {
        return Elements[index];
    }
};

template<typename T>
using Vector4  = Vector<4, T>;
using Vector4f = Vector4<float>;
