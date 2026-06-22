#pragma once

#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <span>
#include <type_traits>


namespace ddknd::math
{
    // =================================== CONSTANTS ===================================
    template<typename T = float>
    inline constexpr T kEpsilon = T(1e-6);
    template<>
    inline constexpr double kEpsilon<double> = 1e-12;
    template<typename T>
    inline constexpr T kEpsilonSq = kEpsilon<T> * kEpsilon<T>;

    // =================================== Linear Algebra =================================== 
    template <typename T, std::size_t N>
    struct Vec
    {
        static_assert(std::is_arithmetic_v<T> && !std::is_same_v<T, bool>);

        std::array<T, N> v{};

        constexpr Vec() = default;

        template <typename... Args>
            requires(sizeof...(Args) == N)
        constexpr Vec(Args... args) : v{static_cast<T>(args)...}
        {
        }

        // accessors
        constexpr T& operator[](std::size_t i) noexcept
        {
            return v[i];
        }
        constexpr const T& operator[](std::size_t i) const noexcept
        {
            return v[i];
        }

        // x, y, z, w accessors
        constexpr T& x() noexcept
            requires(N>=1)
        {
            return v[0];
        }
        constexpr const T& x() const noexcept
            requires(N>=1)
        {
            return v[0];
        }
        constexpr T& y() noexcept
            requires(N>=2)
        {
            return v[1];
        }
        constexpr const T& y() const noexcept
            requires(N>=2)
        {
            return v[1];
        }
        constexpr T& z() noexcept
            requires(N>=3)
        {
            return v[2];
        }
        constexpr const T& z() const noexcept
            requires(N>=3)
        {
            return v[2];
        }
        constexpr T& w() noexcept
            requires(N>=4)
        {
            return v[3];
        }
        constexpr const T& w() const noexcept
            requires(N>=4)
        {
            return v[3];
        }

        constexpr Vec<T, N>& operator+=(const Vec<T, N>& b) noexcept
        {
            for (std::size_t i = 0; i < N; i++)
            {
                v[i] += b[i];
            }
            return *this;
        }

        constexpr Vec<T, N>& operator-=(const Vec<T, N>& b) noexcept
        {
            for (std::size_t i = 0; i < N; i++)
            {
                v[i] -= b[i];
            }
            return *this;
        }

        constexpr Vec<T, N>& operator*=(const T s) noexcept
        {
            for (std::size_t i = 0; i < N; i++)
            {
                v[i] *= s;
            }
            return *this;
        }

        constexpr Vec<T, N>& operator/=(const T s)
        {
            assert(s != T(0));
            for (std::size_t i = 0; i < N; i++)
            {
                v[i] /= s;
            }
            return *this;
        }

        friend constexpr Vec operator+(Vec a, const Vec b) noexcept
        {
            return a += b;
        }

        friend constexpr Vec operator-(Vec a, const Vec b) noexcept
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

        friend constexpr Vec operator/(Vec a, const T s)
        {
            return a /= s;
        }

        friend std::ostream& operator<<(std::ostream& os, const Vec& v)
        {
            os << "[";

            for (std::size_t i = 0; i < N; i++)
            {
                os << v[i];
                if (i + 1 < N)
                    os << ", ";
            }

            os << "]";
            return os;
        }

        static constexpr Vec<T, N> Zero()
        {
            Vec<T, N> out{};
            for (std::size_t i = 0; i < N; i++)
            {
                out[i] = 0;
            }
            return out;
        }

        static constexpr Vec<T, N> One()
        {
            Vec<T, N> out{};
            for (std::size_t i = 0; i < N; i++)
            {
                out[i] = 1;
            }
            return out;
        }
    };

    template<typename T, typename std::size_t N>
    T lengthSquared(const Vec<T, N>& v)
    {
        return dot(v,v);
    }

    template<typename T, typename std::size_t N>
    T length(const Vec<T, N>& v)
    {
        return std::sqrt(dot(v,v));
    }

    template <typename T, typename std::size_t N>
    Vec<T, N> normalize(const Vec<T, N>& v)
    {
        static_assert(std::is_floating_point_v<T>, "normalize accepts only floating-point inputs");

        const T len = std::sqrt(dot(v, v));
        assert(len != T(0));

        return v / len;
    }

    template <typename T, typename std::size_t N>
    T dot(const Vec<T, N>& a, const Vec<T, N>& b)
    {
        T result{};

        for (std::size_t i = 0; i < N; ++i)
            result += a[i] * b[i];

        return result;
    }

    template <typename T>
    constexpr Vec<T, 3> cross(const Vec<T, 3>& a, const Vec<T, 3>& b) noexcept
    {
        return Vec<T, 3>{a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]};
    }

    template <typename T, std::size_t R, std::size_t C>
    struct Mat
    {
        static_assert(std::is_arithmetic_v<T> && !std::is_same_v<T, bool>);
        std::array<T, R * C> v{};

        constexpr Mat() = default;

        template <typename... Args>
            requires(sizeof...(Args) == R * C)
        explicit constexpr Mat(Args... args) : v{static_cast<T>(args)...}
        {
        }

        explicit constexpr Mat(std::initializer_list<T> init)
        {
            assert(init.size() == R * C);
            std::copy(init.begin(), init.end(), v.begin());
        }

        explicit constexpr Mat(std::span<const T, R * C> arr)
        {
            std::copy(arr.begin(), arr.end(), v.begin());
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
            for (std::size_t i = 0; i < R; i++)
            {
                m(i, i) = T(1);
            }
            return m;
        }

        static constexpr Mat Zero() noexcept
        {
            return Mat{};
        }

        friend std::ostream& operator<<(std::ostream& os, const Mat& m)
        {
            for (std::size_t r = 0; r < R; r++)
            {
                os << "[";
                for (std::size_t c = 0; c < C; c++)
                {
                    os << m(r, c) << " ";
                }
                os << "]";
                if (r + 1 != R)
                    os << '\n';
            }
            return os;
        }

        constexpr T* Data() noexcept
        {
            return v.data();
        }

        constexpr const T* Data() const noexcept
        {
            return v.data();
        }
    };

    template <typename T, std::size_t R, std::size_t C, std::size_t K>
    constexpr Mat<T, R, K> operator*(const Mat<T, R, C>& a, const Mat<T, C, K>& b) noexcept
    {
        Mat<T, R, K> res{};
        for (std::size_t r = 0; r < R; r++)
        {
            for (std::size_t k = 0; k < K; k++)
            {
                for (std::size_t c = 0; c < C; c++)
                {
                    res(r, k) += a(r, c) * b(c, k);
                }
            }
        }
        return res;
    }
    template<typename T, std::size_t R, std::size_t C>
    constexpr Vec<T, R> operator*(const Mat<T, R, C>& m, const Vec<T,C>& v) noexcept
    {
        Vec<T, R> res{};

        for(std::size_t r = 0; r < R; r++)
        {
            for(std::size_t c = 0; c < C; c++)
            {
                res[r] += m(r,c) * v[c];
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

        // temporaly
        void Normalize()
        {        
            float magnitude = std::sqrt(w * w + x * x + y * y + z * z);

            // Prevent division by zero
            if (magnitude > 0.00001f)
            {
                float invMag = 1.0f / magnitude;
                w *= invMag;
                x *= invMag;
                y *= invMag;
                z *= invMag;
            }
            else
            {
                // Default to identity quaternion if norm is zero
                w = 1.0f;
                x = 0.0f;
                y = 0.0f;
                z = 0.0f;
            }
        }
    };

    template <typename T>
    constexpr Mat<T, 4, 4> translate(const Vec<T, 3>& t) noexcept
    {
        auto m = Mat<T, 4, 4>::Identity();

        // column-vector convention: M * v
        m(0, 3) = t[0];
        m(1, 3) = t[1];
        m(2, 3) = t[2];

        return m;
    }

    template <typename T>
    constexpr Mat<T, 4, 4> scale(const Vec<T, 3>& s) noexcept
    {
        auto m = Mat<T, 4, 4>::Identity();

        m(0, 0) = s[0];
        m(1, 1) = s[1];
        m(2, 2) = s[2];

        return m;
    }

    template <typename T>
    Mat<T, 4, 4> mat4_cast(Quat<T> q) noexcept
    {
        q.Normalize();

        const T w = q.w;
        const T x = q.x;
        const T y = q.y;
        const T z = q.z;

        Mat<T, 4, 4> m = Mat<T, 4, 4>::Identity();

        m(0, 0) = T(1) - T(2) * (y * y + z * z);
        m(0, 1) = T(2) * (x * y - z * w);
        m(0, 2) = T(2) * (x * z + y * w);

        m(1, 0) = T(2) * (x * y + z * w);
        m(1, 1) = T(1) - T(2) * (x * x + z * z);
        m(1, 2) = T(2) * (y * z - x * w);

        m(2, 0) = T(2) * (x * z - y * w);
        m(2, 1) = T(2) * (y * z + x * w);
        m(2, 2) = T(1) - T(2) * (x * x + y * y);

        return m;
    }
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

        Mat4f ToMatrix() const noexcept
        {
            return ddknd::math::translate(translation)
                * ddknd::math::mat4_cast(rotation)
                * ddknd::math::scale(scale);
        }
    };
} // namespace ddknd::math

// utils
namespace ddknd::math
{
    float degToRadf(float degree);

    Vec3f ComputeForwardVec(float yawRad, float pitchRad);
    Quatf FromAxisAngle(const Vec3f& a, const float rad);

    Vec3f ExtractTranslation(const Mat4f& m);

    Mat4f Inverse(const Mat4f& m);

    Mat4f ExtractRotationOnly(const ddknd::math::Mat4f& m);

    Vec3f TransformPoint(const Mat4f& m, const Vec3f& p);
    Vec3f TransformDirection(const Mat4f& m, const Vec3f& d);
}

// camera
namespace ddknd::math
{
    Mat4f LookAtOpenGLRH(const Vec3f& eye, const Vec3f& target, const Vec3f& up);
    Mat4f PerspectiveOpenGLRH(float fovRad, float aspect, float nearZ, float farZ);
    Mat4f OrthographicOpenGLRH(float left, float right, float bottom, float top, float nearZ, float faZ);
}