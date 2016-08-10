#ifndef MATHLIB_H
#define MATHLIB_H

#include <cmath>

namespace mymath
{
    /**
     * Point
     */
    template <typename T>
    struct Point2d
    {
        T x, y;
        Point2d() = default;
        Point2d(const T& x, const T& y) : x(x), y(y) {}

        bool operator==(const Point2d& other) const
        {
            return x == other.x && y == other.y;
        }

        bool operator!=(const Point2d& other) const
        {
            return !(*this == other);
        }
    };

    /**
     * Vector
     */
    template <typename T>
    struct Vector2d
    {
        T x, y;
        Vector2d() = default;
        Vector2d(const T& x, const T& y) : x(x), y(y) {}

        Vector2d& operator+=(const Vector2d& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        Vector2d& operator-=(const Vector2d& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }

        Vector2d& operator*=(const T& scalar)
        {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        Vector2d& operator/=(const T& scalar)
        {
            x /= scalar;
            y /= scalar;
            return *this;
        }

        bool operator==(const Vector2d& other) const
        {
            return x == other.x && y == other.y;
        }

        bool operator!=(const Vector2d& other) const
        {
            return !(*this == other);
        }

        double length()
        {
            return sqrt(x*x + y*y);
        }

        double angle()
        {
            return atan2(y, x);
        }
    };

    template <typename T>
    inline Vector2d<T> operator+(Vector2d<T> lhs, const Vector2d<T>& rhs)
    {
        return lhs += rhs;
    }

    template <typename T>
    inline Vector2d<T> operator-(Vector2d<T> lhs, const Vector2d<T>& rhs)
    {
        return lhs += rhs;
    }

    template <typename T>
    inline Vector2d<T> operator*(const T& scalar, Vector2d<T> vector)
    {
        return vector *= scalar;
    }

    template <typename T>
    inline Vector2d<T> operator*(Vector2d<T> vector, const T& scalar)
    {
        return vector *= scalar;
    }

    template <typename T>
    inline Vector2d<T> operator/(const T& scalar, Vector2d<T> vector)
    {
        return vector /= scalar;
    }

    template <typename T>
    inline Vector2d<T> operator/(Vector2d<T> vector, const T& scalar)
    {
        return vector /= scalar;
    }

    /**
     * Matrix
     */
    template <typename T>
    struct Matrix2d
    {
        T a11, a12, 
          a21, a22;

        Matrix2d() = default;
        Matrix2d(const T& a11, const T& a12, const T& a21, const T& a22)
            : a11(a11), a12(a12)
            , a21(a21), a22(a22)
            {}

        Matrix2d& operator+=(const Matrix2d rhs)
        {
            a11 += rhs.a11; a12 += rhs.a12;
            a21 += rhs.a21; a22 += rhs.a22;
            return *this;
        }

        Matrix2d& operator-=(const Matrix2d rhs)
        {
            a11 -= rhs.a11; a12 -= rhs.a12;
            a21 -= rhs.a21; a22 -= rhs.a22;
            return *this;
        }

        Matrix2d& operator*=(const Matrix2d rhs)
        {
            a11 = a11 * rhs.a11 + a12 * rhs.a21; a12 = a11 * rhs.a12 + a12 * rhs.a22;
            a21 = a21 * rhs.a11 + a22 * rhs.a21; a22 = a21 * rhs.a12 + a22 * rhs.a22;
            return *this;
        }

        Matrix2d& operator*=(const T& scalar)
        {
            a11 *= scalar; a12 *= scalar;
            a21 *= scalar; a22 *= scalar;
            return *this;
        }

        bool operator==(const Matrix2d& other) const
        {
            return a11 == other.a11 && a12 == other.a12 &&
                   a21 == other.a21 && a22 == other.a22;
        }

        bool operator!=(const Matrix2d& other) const
        {
            return !(*this == other);
        }
    };

    template <typename T>
    inline Matrix2d<T> operator+(Matrix2d<T> lhs, const Matrix2d<T>& rhs)
    {
        return lhs += rhs;
    }

    template <typename T>
    inline Matrix2d<T> operator-(Matrix2d<T> lhs, const Matrix2d<T>& rhs)
    {
        return lhs -= rhs;
    }

    template <typename T>
    inline Matrix2d<T> operator*(Matrix2d<T>& lhs, const Matrix2d<T>& rhs)
    {
        return lhs *= rhs;
    }

    template <typename T>
    inline Matrix2d<T> operator*(const T& scalar, Matrix2d<T> matrix)
    {
        return matrix *= scalar;
    }

    template <typename T>
    inline Matrix2d<T> operator*(Matrix2d<T> matrix, const T& scalar)
    {
        return matrix *= scalar;
    }

    const Matrix2d<double> IDENTITY_MATRIX = {
        1.0, 0.0,
        0.0, 1.0
    };

    template <typename T>
    Matrix2d<T> rotationMatrix(const T& x)
    {
        return {
            cos(x), -sin(x),
            sin(x), cos(x)
        };
    }

    template <typename ROTATABLE, typename ROTATION>
    ROTATABLE rotate(const ROTATABLE& rotatable, const ROTATION& rotation)
    {
        return rotatable * rotationMatrix(rotation);
    }

    template <typename T>
    Vector2d<T> operator*(Matrix2d<T> matrix, const Vector2d<T>& vector)
    {
        return {
            vector.x * matrix.a11 + vector.y * matrix.a12,
            vector.x * matrix.a21 + vector.y * matrix.a22
        };
    }

    template <typename T>
    Vector2d<T> operator*(const Vector2d<T>& vector, Matrix2d<T> matrix)
    {
        return {
            vector.x * matrix.a11 + vector.y * matrix.a12,
            vector.x * matrix.a21 + vector.y * matrix.a22
        };
    }
}

#endif
