#pragma once

#include "Vector2.h"

// TODO: Can be improved with SIMD

template <typename T>
class Matrix2
{
public:
    static Matrix2<T> calculateRotationMatrix(const T& x);

    T a11, a12, a21, a22;

    Matrix2();
    Matrix2(const T& a11, const T& a12, const T& a21, const T& a22);

    Matrix2& operator+=(const Matrix2& other);
    Matrix2& operator-=(const Matrix2& other);
    Matrix2& operator*=(const Matrix2& other);
    Matrix2& operator*=(const T& scalar);

    bool operator==(const Matrix2& other) const;
    bool operator!=(const Matrix2& other) const;
};

namespace Matrix
{
template <typename Rotatable, typename Rotation>
static Rotatable rotate(const Rotatable& rotatable, const Rotation& rotation);
}

template <typename T>
inline Matrix2<T> operator+(Matrix2<T> lhs, const Matrix2<T>& rhs);

template <typename T>
inline Matrix2<T> operator-(Matrix2<T> lhs, const Matrix2<T>& rhs);

template <typename T>
inline Matrix2<T> operator*(Matrix2<T>& lhs, const Matrix2<T>& rhs);

template <typename T>
inline Matrix2<T> operator*(const T& scalar, Matrix2<T> matrix);

template <typename T>
inline Matrix2<T> operator*(Matrix2<T> matrix, const T& scalar);

template <typename T>
inline Vector2<T> operator*(Matrix2<T> matrix, const Vector2<T>& vector);

template <typename T>
inline Vector2<T> operator*(const Vector2<T>& vector, Matrix2<T> matrix);

#include "Matrix2.inl"

