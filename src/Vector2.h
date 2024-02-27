#pragma once

template <typename T>
struct Vector2
{
    constexpr Vector2() = default;
    constexpr Vector2(const T& x, const T& y) : x(x), y(y) {}

    Vector2& operator+=(const Vector2& other);
    Vector2& operator-=(const Vector2& other);
    Vector2& operator*=(const T& scalar);
    Vector2& operator/=(const T& scalar);

    bool operator==(const Vector2& other) const;
    bool operator!=(const Vector2& other) const;

    template <typename E>
    operator Vector2<E>() const;

    float length();
    float angle();

    T x, y;
};

template <typename T>
inline Vector2<T> operator+(Vector2<T> lhs, const Vector2<T>& rhs);

template <typename T>
inline Vector2<T> operator-(Vector2<T> lhs, const Vector2<T>& rhs);

template <typename T>
inline Vector2<T> operator*(const T& scalar, Vector2<T> vector);

template <typename T>
inline Vector2<T> operator*(Vector2<T> vector, const T& scalar);

template <typename T>
inline Vector2<T> operator/(const T& scalar, Vector2<T> vector);

template <typename T>
inline Vector2<T> operator/(Vector2<T> vector, const T& scalar);

#include "Vector2.inl"

