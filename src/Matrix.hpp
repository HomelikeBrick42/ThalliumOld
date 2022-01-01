#pragma once

#include "Vector.hpp"

template<size_t Row, size_t Col, typename T>
using Matrix = Vector<Col, Vector<Row, T>>;

template<typename T>
using Matrix4x4 = Matrix<4, 4, T>;

template<size_t Row1, size_t Col1Row2, size_t Col2, typename T>
inline constexpr Matrix<Row1, Col2, T> operator*(const Matrix<Row1, Col1Row2, T>& a, const Matrix<Col1Row2, Col2, T>& b) {
    Matrix<Row1, Col2, T> result;
    for (size_t i = 0; i < Row1; i++) {
        for (size_t j = 0; j < Col2; j++) {
            T sum{};
            for (size_t k = 0; k < Col1Row2; k++) {
                sum += a[i][k] * b[k][j];
            }
            result[i][j] = sum;
        }
    }
    return result;
}

template<size_t Row, size_t Length, typename T>
Vector<Length, T> operator*(const Matrix<Row, Length, T>& m, const Vector<Length, T>& v) {
    Matrix<Length, 1, T> b;
    for (size_t i = 0; i < Length; i++) {
        b[i][0] = v[i];
    }

    Matrix<Length, 1, T> matrixResult = m * b;

    Vector<Length, T> result;
    for (size_t i = 0; i < Length; i++) {
        result[i] = matrixResult[i][0];
    }
    return result;
}

template<size_t Row, size_t Length, typename T>
Vector<Length, T> operator*(const Vector<Length, T>& v, const Matrix<Row, Length, T>& m) {
    Matrix<Length, 1, T> b;
    for (size_t i = 0; i < Length; i++) {
        b[i][0] = v[i];
    }

    Matrix<Length, 1, T> matrixResult = b * m;

    Vector<Length, T> result;
    for (size_t i = 0; i < Length; i++) {
        result[i] = matrixResult[i][0];
    }
    return result;
}

template<typename T>
using Matrix4x4  = Matrix<4, 4, T>;
using Matrix4x4f = Matrix4x4<float>;

inline constexpr Matrix4x4f Matrix4x4f_Identity() {
    return {
        Vector4f{ 1.0f, 0.0f, 0.0f, 0.0f },
        Vector4f{ 0.0f, 1.0f, 0.0f, 0.0f },
        Vector4f{ 0.0f, 0.0f, 1.0f, 0.0f },
        Vector4f{ 0.0f, 0.0f, 0.0f, 1.0f },
    };
}
