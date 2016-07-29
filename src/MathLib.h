#ifndef MATHLIB_H
#define MATHLIB_H

namespace mymath
{
    /**
     * Point
     */
    template <typename T>
    struct Point2d
    {
        T x, y;
        Point2d(const T& x, const T& y) : x(x), y(y) {}
    };

    /**
     * Vector
     */
    template <typename T>
    struct Vector2d
    {
        T x, y;
        Vector2d(const T& x, const T& y) : x(x), y(y) {}
    };

    template <typename T>
    Vector2d<T> operator+(const Vector2d<T>& lhs, const Vector2d<T>& rhs)
    {
        return {
            lhs.x + rhs.x,
            lhs.y + rhs.y
        };
    }

    template <typename T>
    Vector2d<T> operator-(const Vector2d<T>& lhs, const Vector2d<T>& rhs)
    {
        return {
            lhs.x - rhs.x,
            lhs.y - rhs.y
        };
    }

    template <typename T>
    Vector2d<T> operator*(const T& scalar, const Vector2d<T>& vector)
    {
        return { scalar * vector.x, scalar * vector.y };
    }

    template <typename T>
    Vector2d<T> operator*(const Vector2d<T>& vector, const T& scalar)
    {
        return { scalar * vector.x, scalar * vector.y };
    }

    /**
     * Matrix
     */
    template <typename T>
    struct Matrix2d
    {
        T a11, a12, 
          a21, a22;
        Matrix2d(const T& a11, const T& a12, const T& a21, const T& a22)
            : a11(a11), a12(a12)
            , a21(a21), a22(a22)
            {}
    };

    template <typename T>
    Matrix2d<T> operator+(const Matrix2d<T>& lhs, const Matrix2d<T>& rhs)
    {
        return {
            lhs.a11 + rhs.a11, lhs.a12 + rhs.a12,
            lhs.a21 + rhs.a21, lhs.a22 + rhs.a22
        };
    }

    template <typename T>
    Matrix2d<T> operator-(const Matrix2d<T>& lhs, const Matrix2d<T>& rhs)
    {
        return {
            lhs.a11 - rhs.a11, lhs.a12 - rhs.a12,
            lhs.a21 - rhs.a21, lhs.a22 - rhs.a22
        };
    }

    template <typename T>
    Matrix2d<T> operator*(const Matrix2d<T>& lhs, const Matrix2d<T>& rhs)
    {
        return {
            (lhs.a11 * rhs.a11 + lhs.a12 * rhs.a21), (lhs.a11 * rhs.a12 + lhs.a12 * rhs.a22),
            (lhs.a21 * rhs.a11 + lhs.a22 * rhs.a21), (lhs.a21 * rhs.a12 + lhs.a22 * rhs.a22)
        };
    }

    template <typename T>
    Matrix2d<T> operator*(const T& scalar, const Matrix2d<T>& matrix)
    {
        return {
            scalar * matrix.a11, scalar * matrix.a12,
            scalar * matrix.a21, scalar * matrix.a22
        };
    }

    template <typename T>
    Matrix2d<T> operator*(const Matrix2d<T>& matrix, const T& scalar)
    {
        return {
            scalar * matrix.a11, scalar * matrix.a12,
            scalar * matrix.a21, scalar * matrix.a22
        };
    }

    const Matrix2d<double> IDENTITY_MATRIX = {
        1, 0,
        0, 1
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
