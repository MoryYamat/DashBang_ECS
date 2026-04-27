#pragma once

#include <iostream>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace ddknd::math
{
    template <typename T, std::size_t N>
    struct Vec;

    template <typename T>
    struct Vec<T, 2>
    {
        static_assert(std::is_arithmetic_v<T> && !std::is_same_v<T, bool>);
        T x{};
        T y{};

        constexpr Vec() = default;
        constexpr Vec(T x_, T y_) : x(x_), y(y_) {}
        constexpr Vec(const Vec& v_) = default;

        constexpr Vec& operator+=(const Vec& o) noexcept
        {
            x += o.x;
            y += o.y;
            return *this;
        }
        constexpr Vec& operator-=(const Vec& o) noexcept
        {
            x -= o.x;
            y -= o.y;
            return *this;
        }
        constexpr Vec& operator*=(const T s) noexcept
        {
            x *= s;
            y *= s;
            return *this;
        }
        Vec& operator/=(const T s)
        {
            assert(s != T(0));
            x /= s;
            y /= s;
            return *this;
        }

        friend constexpr Vec operator+(Vec a, const Vec& b) noexcept
        {
            return a += b;
        }
        friend constexpr Vec operator-(Vec a, const Vec& b) noexcept
        {
            return a -= b;
        }
        friend constexpr Vec operator*(Vec a, const T s) noexcept
        {
            return a *= s;
        }
        friend constexpr Vec operator*(T s, Vec a) noexcept
        {
            return a *= s;
        }
        friend Vec operator/(Vec a, const T s)
        {
            return a /= s;
        }

        // unary minus
        friend constexpr Vec operator-(Vec v) noexcept
        {
            v.x = -v.x;
            v.y = -v.y;
            return v;
        }

        static constexpr Vec Zero() noexcept
        {
            return Vec{T(0), T(0)};
        }
        static constexpr Vec One() noexcept
        {
            return Vec{T(1), T(1)};
        }
    };

    template <typename T>
    struct Vec<T, 3>
    {
        static_assert(std::is_arithmetic_v<T> && !std::is_same_v<T, bool>);
        T x{};
        T y{};
        T z{};

        constexpr Vec() = default;
        constexpr Vec(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}

        constexpr Vec& operator+=(const Vec& o) noexcept
        {
            x += o.x;
            y += o.y;
            z += o.z;
            return *this;
        }
        constexpr Vec& operator-=(const Vec& o) noexcept
        {
            x -= o.x;
            y -= o.y;
            z -= o.z;
            return *this;
        }
        constexpr Vec& operator*=(const T s) noexcept
        {
            x *= s;
            y *= s;
            z *= s;
            return *this;
        }
        Vec& operator/=(const T s)
        {
            assert(s != T(0));
            x /= s;
            y /= s;
            z /= s;
            return *this;
        }

        friend constexpr Vec operator+(Vec a, const Vec& b) noexcept
        {
            return a += b;
        }
        friend constexpr Vec operator-(Vec a, const Vec& b) noexcept
        {
            return a -= b;
        }
        friend constexpr Vec operator*(Vec a, const T s) noexcept
        {
            return a *= s;
        }
        friend constexpr Vec operator*(T s, Vec a) noexcept
        {
            return a *= s;
        }
        friend Vec operator/(Vec a, const T s)
        {
            return a /= s;
        }

        // unary minus
        friend constexpr Vec operator-(Vec v) noexcept
        {
            v.x = -v.x;
            v.y = -v.y;
            v.z = -v.z;
            return v;
        }

        static constexpr Vec Zero() noexcept
        {
            return Vec{T(0), T(0), T(0)};
        }
        static constexpr Vec One() noexcept
        {
            return Vec{T(1), T(1), T(1)};
        }
    };

    template <typename T>
    struct Vec<T, 4>
    {
        static_assert(std::is_arithmetic_v<T> && !std::is_same_v<T, bool>);
        T x{};
        T y{};
        T z{};
        T w{};

        constexpr Vec() = default;
        constexpr Vec(T x_, T y_, T z_, T w_) : x(x_), y(y_), z(z_), w(w_) {}

        constexpr Vec& operator+=(const Vec& o) noexcept
        {
            x += o.x;
            y += o.y;
            z += o.z;
            w += o.w;
            return *this;
        }
        constexpr Vec& operator-=(const Vec& o) noexcept
        {
            x -= o.x;
            y -= o.y;
            z -= o.z;
            w -= o.w;
            return *this;
        }
        constexpr Vec& operator*=(const T s) noexcept
        {
            x *= s;
            y *= s;
            z *= s;
            w *= s;
            return *this;
        }
        Vec& operator/=(const T s)
        {
            assert(s != T(0));
            x /= s;
            y /= s;
            z /= s;
            w /= s;
            return *this;
        }

        friend constexpr Vec operator+(Vec a, const Vec& b) noexcept
        {
            return a += b;
        }
        friend constexpr Vec operator-(Vec a, const Vec& b) noexcept
        {
            return a -= b;
        }
        friend constexpr Vec operator*(Vec a, const T s) noexcept
        {
            return a *= s;
        }
        friend constexpr Vec operator*(T s, Vec a) noexcept
        {
            return a *= s;
        }
        friend Vec operator/(Vec a, const T s)
        {
            return a /= s;
        }

        // unary minus
        friend constexpr Vec operator-(Vec v) noexcept
        {
            v.x = -v.x;
            v.y = -v.y;
            v.z = -v.z;
            v.w = -v.w;
            return v;
        }

        static constexpr Vec Zero() noexcept
        {
            return Vec{T(0), T(0), T(0), T(0)};
        }
        static constexpr Vec One() noexcept
        {
            return Vec{T(1), T(1), T(1), T(1)};
        }
    };

    template <typename T, std::size_t R, std::size_t C>
    struct Mat
    {
        static_assert(std::is_arithmetic_v<T> && !std::is_same_v<T, bool>);
        std::array<T, R * C> v{};

        constexpr Mat() = default;

        explicit constexpr Mat(std::initializer_list<T> init)
        {
            assert(init.size() == R * C);
            std::copy(init.begin(), init.end(), v.begin());
        }

        constexpr T& operator()(std::size_t r, std::size_t c) noexcept
        {
            return v[r * C + c]; // row-major
        }

        constexpr const T& operator()(std::size_t r, std::size_t c) const noexcept
        {
            return v[r * C + c];
        }

        static constexpr Mat Identity() noexcept
        {
            static_assert(R == C, "Identity is only defined for square matrices");

            Mat m{};
            for(std::size_t i = 0; i < R; i++)
            {
                m(i,i) = T(1);
            }
            return m;
        }

        static constexpr Mat Zero() noexcept
        {
            return Mat{};
        }

        friend std::ostream& operator<<(std::ostream& os, const Mat& m)
        {
            for(std::size_t r = 0; r < R; r++)
            {
                os << "[";
                for(std::size_t c = 0; c < C; c++)
                {
                    os << m(r,c) << " ";
                }
                os << "]";
                if(r + 1 != R) os << '\n';
            }
            return os;
        }
    };


    template<typename T, std::size_t R, std::size_t C, std::size_t K>
    constexpr Mat<T,R,K> operator*(const Mat<T,R,C>& a, const Mat<T, C, K>& b) noexcept
    {
        Mat<T, R, K> res{};
        for(std::size_t r = 0; r < R; r++)
        {
            for(std::size_t k = 0; k < K; k++)
            {
                for(std::size_t c = 0; c < C; c++)
                {
                    res(r,k) += a(r,c) * b(c,k);
                }
            }
        }
        return res;
    }

    template <typename T>
    struct Quat
    {
        static_assert(std::is_floating_point_v<T>);
        T w{T(1)};
        T x{T(0)};
        T y{T(0)};
        T z{T(0)};

        constexpr Quat() = default;
        constexpr Quat(T w_, T x_, T y_, T z_) : w(w_), x(x_), y(y_), z(z_) {}
        static constexpr Quat Identity() noexcept
        {
            return Quat{T(1), T(0), T(0), T(0)};
        }
    };
    // need to implement some other type and operations (e.g. matrix / rotation / convert to glm)
} // namespace ddknd::math

// alias
namespace ddknd::math
{
    using uVec2 = Vec<std::uint32_t, 2>;
    using uVec3 = Vec<std::uint32_t, 3>;
    using uVec4 = Vec<std::uint32_t, 4>;

    using u64Vec2 = Vec<std::uint64_t, 2>;
    using u64Vec3 = Vec<std::uint64_t, 3>;
    using u64Vec4 = Vec<std::uint64_t, 4>;

    using Vec2f = Vec<float, 2>;
    using Vec3f = Vec<float, 3>;
    using Vec4f = Vec<float, 4>;

    using Mat3f = Mat<float, 3, 3>;
    using Mat4f = Mat<float, 4, 4>;

    using Quatf = Quat<float>;

    struct TRS
    {
        Vec3f translation = Vec3f::Zero();
        Quatf rotation = Quatf::Identity();
        Vec3f scale = Vec3f::One();
    };
} // namespace ddknd::math