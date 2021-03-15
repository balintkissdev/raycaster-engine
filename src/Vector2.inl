template <typename T>
Vector2<T>& Vector2<T>::operator+=(const Vector2<T>& other)
{
    x += other.x;
    y += other.y;
    return *this;
}

template <typename T>
Vector2<T>& Vector2<T>::operator-=(const Vector2<T>& other)
{
    x -= other.x;
    y -= other.y;
    return *this;
}

template <typename T>
Vector2<T>& Vector2<T>::operator*=(const T& scalar)
{
    x *= scalar;
    y *= scalar;
    return *this;
}

template <typename T>
Vector2<T>& Vector2<T>::operator/=(const T& scalar)
{
    x /= scalar;
    y /= scalar;
    return *this;
}

template <typename T>
bool Vector2<T>::operator==(const Vector2& other) const
{
    return x == other.x && y == other.y;
}

template <typename T>
bool Vector2<T>::operator!=(const Vector2& other) const
{
    return !(*this == other);
}

template <typename T>
template <typename E>
Vector2<T>::operator Vector2<E>() const
{
    return {static_cast<E>(x), static_cast<E>(y)};
}

template <typename T>
float Vector2<T>::length()
{
    return sqrtf(x * x + y * y);
}

template <typename T>
float Vector2<T>::angle()
{
    return atan2(y, x);
}

template <typename T>
Vector2<T> operator+(Vector2<T> lhs, const Vector2<T>& rhs)
{
    return lhs += rhs;
}

template <typename T>
Vector2<T> operator-(Vector2<T> lhs, const Vector2<T>& rhs)
{
    return lhs -= rhs;
}

template <typename T>
Vector2<T> operator*(const T& scalar, Vector2<T> vector)
{
    return vector *= scalar;
}

template <typename T>
Vector2<T> operator*(Vector2<T> vector, const T& scalar)
{
    return vector *= scalar;
}

template <typename T>
Vector2<T> operator/(const T& scalar, Vector2<T> vector)
{
    return vector /= scalar;
}

template <typename T>
Vector2<T> operator/(Vector2<T> vector, const T& scalar)
{
    return vector /= scalar;
}