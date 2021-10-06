/*
 * GraphMath @ https://github.com/yiyaowen/GraphMath
 * yiyaowen 2021 (c) All Rights Reserved.
 *
 * Import graphics math to existing projects seamlessly.
 */

#ifndef VECTOR_3_HPP
#define VECTOR_3_HPP

#include <array>
#include <cmath>
#include <iostream>

namespace gmath {
    template<typename T>
    struct Vector3 {
    public:
        Vector3() : m_array { 0, 0, 0 } {}
        Vector3(T c0, T c1, T c2) : m_array { c0, c1, c2 } {}

    public: // Operators
        inline T operator[](size_t i) const { return m_array[i]; }
        inline T& operator[](size_t i) { return m_array[i]; };

        inline Vector3<T> operator-() const { return { -m_array[0], -m_array[1], -m_array[2] }; }

        inline friend std::ostream& operator<<(std::ostream& out, const Vector3<T>& v3) {
            return out << v3.m_array[0] << ' ' << v3.m_array[1] << ' ' << v3.m_array[2];
        }

        inline friend Vector3<T> operator+(const Vector3<T>& u3, const Vector3<T>& v3) {
            return { u3.m_array[0] + v3.m_array[0], u3.m_array[1] + v3.m_array[1], u3.m_array[2] + v3.m_array[2] };
        }

        inline friend Vector3<T> operator-(const Vector3<T>& u3, const Vector3<T>& v3) {
            return { u3.m_array[0] - v3.m_array[0], u3.m_array[1] - v3.m_array[1], u3.m_array[2] - v3.m_array[2] };
        }

        inline friend Vector3<T> operator*(const Vector3<T>& u3, const Vector3<T>& v3) {
            return { u3.m_array[0] * v3.m_array[0], u3.m_array[1] * v3.m_array[1], u3.m_array[2] * v3.m_array[2] };
        }

        inline friend Vector3<T> operator*(const T& t, const Vector3<T>& v3) {
            return { t * v3.m_array[0], t * v3.m_array[1], t * v3.m_array[2] };
        }

        inline friend Vector3<T> operator*(const Vector3<T>& v3, const T& t) {
            return t * v3;
        }

        inline friend Vector3<T> operator/(const Vector3<T>& v3, const T& t) {
            return (1/t) * v3;
        }

        inline Vector3<T>& operator+=(const Vector3<T>& v3) {
            m_array[0] += v3[0];
            m_array[1] += v3[1];
            m_array[2] += v3[2];
            return *this;
        }

        inline Vector3<T>& operator*=(const T& t) {
            m_array[0] *= t;
            m_array[1] *= t;
            m_array[2] *= t;
            return *this;
        }

        inline Vector3<T>& operator/=(const T& t) {
            m_array[0] /= t;
            m_array[1] /= t;
            m_array[2] /= t;
        }

    public: // Tools
        inline T length() const {
            return sqrt(length2());
        }

        inline T length2() const {
            return m_array[0] * m_array[0] + m_array[1] * m_array[1] + m_array[2] * m_array[2];
        }

        inline friend T dot(const Vector3<T>& u3, const Vector3<T>& v3) {
            return u3.m_array[0] * v3.m_array[0] + u3.m_array[1] * v3.m_array[1] + u3.m_array[2] * v3.m_array[2];
        }

        inline friend Vector3<T> cross(const Vector3<T>& u3, const Vector3<T>& v3) {
            return { u3.m_array[1] * v3.m_array[2] - u3.m_array[2] * v3.m_array[1],
                     u3.m_array[2] * v3.m_array[0] - u3.m_array[0] * v3.m_array[2],
                     u3.m_array[0] * v3.m_array[1] - u3.m_array[1] * v3.m_array[0] };
        }

        inline friend Vector3<T> normalize(const Vector3<T>& v3) {
            return v3 / v3.length();
        }

        inline bool nearZero() const {
            constexpr double epsilon = 1e-8;
            return (abs(m_array[0]) < epsilon) && (abs(m_array[1]) < epsilon) && (abs(m_array[2]) < epsilon);
        }

        inline friend Vector3<T> reflect(const Vector3<T>& v, const Vector3<T>& n) {
            return v - 2 * dot(v, n) * n;
        }

        inline friend Vector3<T> refract(const Vector3<T>& v, const Vector3<T>& n, T refractiveIndex) {
            T cosTheta = dot(n, -v);
            Vector3<T> refractedPerpendicular = (v + cosTheta * n) * refractiveIndex;
            Vector3<T> refractedParallel = -sqrt(abs(1.0 - refractedPerpendicular.length2())) * n;
            return refractedPerpendicular + refractedParallel;
        }

    public: // Static tools
        inline static Vector3<T> zero() {
            return { 0, 0, 0 };
        }

    public: // Swizzle getter and setter
        inline T x() const { return m_array[0]; }
        inline T y() const { return m_array[1]; }
        inline T z() const { return m_array[2]; }

        inline T r() const { return x(); }
        inline T g() const { return y(); }
        inline T b() const { return z(); }

    private:
        std::array<T, 3> m_array = {};
    };

    // Type alias
    using Vector3i = Vector3<int>;
    using Vector3f = Vector3<float>;
    using Vector3d = Vector3<double>;
}

#endif // VECTOR_3_HPP