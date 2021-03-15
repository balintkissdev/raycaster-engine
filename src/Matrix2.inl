#include <cmath>

template <typename T>
Matrix2<T>::Matrix2() = default;

template <typename T>
Matrix2<T>::Matrix2(const T& a11, const T& a12, const T& a21, const T& a22) : a11(a11), a12(a12), a21(a21), a22(a22)
{
}

template <typename T>
Matrix2<T>& Matrix2<T>::operator+=(const Matrix2<T>& other)
{
    a11 += other.a11;
    a12 += other.a12;
    a21 += other.a21;
    a22 += other.a22;
    return *this;
}

template <typename T>
Matrix2<T>& Matrix2<T>::operator-=(const Matrix2<T>& other)
{
    a11 -= other.a11;
    a12 -= other.a12;
    a21 -= other.a21;
    a22 -= other.a22;
    return *this;
}

template <typename T>
Matrix2<T>& Matrix2<T>::operator*=(const Matrix2<T>& other)
{
    a11 = a11 * other.a11 + a12 * other.a21;
    a12 = a11 * other.a12 + a12 * other.a22;
    a21 = a21 * other.a11 + a22 * other.a21;
    a22 = a21 * other.a12 + a22 * other.a22;
    return *this;
}

template <typename T>
Matrix2<T>& Matrix2<T>::operator*=(const T& scalar)
{
    a11 *= scalar;
    a12 *= scalar;
    a21 *= scalar;
    a22 *= scalar;
    return *this;
}

template <typename T>
bool Matrix2<T>::operator==(const Matrix2<T>& other) const
{
    return a11 == other.a11 && a12 == other.a12 && a21 == other.a21 && a22 == other.a22;
}

template <typename T>
bool Matrix2<T>::operator!=(const Matrix2<T>& other) const
{
    return !(*this == other);
}

template <typename T>
inline Matrix2<T> operator+(Matrix2<T> lhs, const Matrix2<T>& rhs)
{
    return lhs += rhs;
}

template <typename T>
inline Matrix2<T> operator-(Matrix2<T> lhs, const Matrix2<T>& rhs)
{
    return lhs -= rhs;
}

template <typename T>
inline Matrix2<T> operator*(Matrix2<T>& lhs, const Matrix2<T>& rhs)
{
    return lhs *= rhs;
}

template <typename T>
inline Matrix2<T> operator*(const T& scalar, Matrix2<T> matrix)
{
    return matrix *= scalar;
}

template <typename T>
inline Matrix2<T> operator*(Matrix2<T> matrix, const T& scalar)
{
    return matrix *= scalar;
}

template <typename T>
Matrix2<T> Matrix2<T>::calculateRotationMatrix(const T& x)
{
    return {cosf(x), -sinf(x), sinf(x), cosf(x)};
}

template <typename Rotatable, typename Rotation>
Rotatable Matrix::rotate(const Rotatable& rotatable, const Rotation& rotation)
{
    return rotatable * Matrix2<Rotation>::calculateRotationMatrix(rotation);
}

template <typename T>
Vector2<T> operator*(Matrix2<T> matrix, const Vector2<T>& vector)
{
    return {vector.x * matrix.a11 + vector.y * matrix.a12, vector.x * matrix.a21 + vector.y * matrix.a22};
}

template <typename T>
Vector2<T> operator*(const Vector2<T>& vector, Matrix2<T> matrix)
{
    return {vector.x * matrix.a11 + vector.y * matrix.a12, vector.x * matrix.a21 + vector.y * matrix.a22};
}