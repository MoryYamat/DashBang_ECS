#include "ddknd/math/math.h"

#include <cassert>
#include <cmath>
#include <numbers>

namespace ddknd::math
{
    float degToRadf(float degree)
    {
        return degree * (std::numbers::pi_v<float> / 180.0f);
    }

    Vec3f ComputeForwardVec(float yawRad, float pitchRad)
    {
        Vec3f forward;
        forward[0] = std::cos(yawRad) * std::cos(pitchRad);
        forward[1] = std::sin(pitchRad);
        forward[2] = std::sin(yawRad) * std::cos(pitchRad);

        return forward;
    }
    Quatf FromAxisAngle(const Vec3f& a, const float rad)
    {
        Vec3f n = normalize(a);

        float half = rad / 2;
        float s = std::sin(half);

        return {std::cos(half), n.x() * s, n.y() * s, n.z() * s};
    }

    Vec3f ExtractTranslation(const Mat4f& m)
    {
        return {m(0, 3), m(1, 3), m(2, 3)};
    }

    Mat4f Inverse(const Mat4f& m)
    {
        const float* a = m.Data();

        Mat4f inv{};

        inv.v[0] = a[5] * a[10] * a[15] - a[5] * a[11] * a[14] - a[9] * a[6] * a[15] + a[9] * a[7] * a[14] +
                   a[13] * a[6] * a[11] - a[13] * a[7] * a[10];

        inv.v[4] = -a[4] * a[10] * a[15] + a[4] * a[11] * a[14] + a[8] * a[6] * a[15] - a[8] * a[7] * a[14] -
                   a[12] * a[6] * a[11] + a[12] * a[7] * a[10];

        inv.v[8] = a[4] * a[9] * a[15] - a[4] * a[11] * a[13] - a[8] * a[5] * a[15] + a[8] * a[7] * a[13] +
                   a[12] * a[5] * a[11] - a[12] * a[7] * a[9];

        inv.v[12] = -a[4] * a[9] * a[14] + a[4] * a[10] * a[13] + a[8] * a[5] * a[14] - a[8] * a[6] * a[13] -
                    a[12] * a[5] * a[10] + a[12] * a[6] * a[9];

        inv.v[1] = -a[1] * a[10] * a[15] + a[1] * a[11] * a[14] + a[9] * a[2] * a[15] - a[9] * a[3] * a[14] -
                   a[13] * a[2] * a[11] + a[13] * a[3] * a[10];

        inv.v[5] = a[0] * a[10] * a[15] - a[0] * a[11] * a[14] - a[8] * a[2] * a[15] + a[8] * a[3] * a[14] +
                   a[12] * a[2] * a[11] - a[12] * a[3] * a[10];

        inv.v[9] = -a[0] * a[9] * a[15] + a[0] * a[11] * a[13] + a[8] * a[1] * a[15] - a[8] * a[3] * a[13] -
                   a[12] * a[1] * a[11] + a[12] * a[3] * a[9];

        inv.v[13] = a[0] * a[9] * a[14] - a[0] * a[10] * a[13] - a[8] * a[1] * a[14] + a[8] * a[2] * a[13] +
                    a[12] * a[1] * a[10] - a[12] * a[2] * a[9];

        inv.v[2] = a[1] * a[6] * a[15] - a[1] * a[7] * a[14] - a[5] * a[2] * a[15] + a[5] * a[3] * a[14] +
                   a[13] * a[2] * a[7] - a[13] * a[3] * a[6];

        inv.v[6] = -a[0] * a[6] * a[15] + a[0] * a[7] * a[14] + a[4] * a[2] * a[15] - a[4] * a[3] * a[14] -
                   a[12] * a[2] * a[7] + a[12] * a[3] * a[6];

        inv.v[10] = a[0] * a[5] * a[15] - a[0] * a[7] * a[13] - a[4] * a[1] * a[15] + a[4] * a[3] * a[13] +
                    a[12] * a[1] * a[7] - a[12] * a[3] * a[5];

        inv.v[14] = -a[0] * a[5] * a[14] + a[0] * a[6] * a[13] + a[4] * a[1] * a[14] - a[4] * a[2] * a[13] -
                    a[12] * a[1] * a[6] + a[12] * a[2] * a[5];

        inv.v[3] = -a[1] * a[6] * a[11] + a[1] * a[7] * a[10] + a[5] * a[2] * a[11] - a[5] * a[3] * a[10] -
                   a[9] * a[2] * a[7] + a[9] * a[3] * a[6];

        inv.v[7] = a[0] * a[6] * a[11] - a[0] * a[7] * a[10] - a[4] * a[2] * a[11] + a[4] * a[3] * a[10] +
                   a[8] * a[2] * a[7] - a[8] * a[3] * a[6];

        inv.v[11] = -a[0] * a[5] * a[11] + a[0] * a[7] * a[9] + a[4] * a[1] * a[11] - a[4] * a[3] * a[9] -
                    a[8] * a[1] * a[7] + a[8] * a[3] * a[5];

        inv.v[15] = a[0] * a[5] * a[10] - a[0] * a[6] * a[9] - a[4] * a[1] * a[10] + a[4] * a[2] * a[9] +
                    a[8] * a[1] * a[6] - a[8] * a[2] * a[5];

        const float det = a[0] * inv.v[0] + a[1] * inv.v[4] + a[2] * inv.v[8] + a[3] * inv.v[12];

        if (std::abs(det) < 1e-8f)
        {
            return Mat4f::Identity();
        }

        const float invDet = 1.0f / det;

        for (float& x : inv.v)
        {
            x *= invDet;
        }

        return inv;
    }

    Mat4f ExtractRotationOnly(const ddknd::math::Mat4f& m)
    {
        using ddknd::math::Mat4f;

        Mat4f r = Mat4f::Identity();

        // column 0
        {
            float x = m(0, 0);
            float y = m(1, 0);
            float z = m(2, 0);

            float len = std::sqrt(x * x + y * y + z * z);
            if (len > 1e-8f)
            {
                r(0, 0) = x / len;
                r(1, 0) = y / len;
                r(2, 0) = z / len;
            }
        }

        // column 1
        {
            float x = m(0, 1);
            float y = m(1, 1);
            float z = m(2, 1);

            float len = std::sqrt(x * x + y * y + z * z);
            if (len > 1e-8f)
            {
                r(0, 1) = x / len;
                r(1, 1) = y / len;
                r(2, 1) = z / len;
            }
        }

        // column 2
        {
            float x = m(0, 2);
            float y = m(1, 2);
            float z = m(2, 2);

            float len = std::sqrt(x * x + y * y + z * z);
            if (len > 1e-8f)
            {
                r(0, 2) = x / len;
                r(1, 2) = y / len;
                r(2, 2) = z / len;
            }
        }

        r(0, 3) = 0.0f;
        r(1, 3) = 0.0f;
        r(2, 3) = 0.0f;

        r(3, 0) = 0.0f;
        r(3, 1) = 0.0f;
        r(3, 2) = 0.0f;
        r(3, 3) = 1.0f;

        return r;
    }

    Vec3f TransformPoint(const Mat4f& m, const Vec3f& p)
    {
        Vec4f v = m * Vec4f{p.x(), p.y(), p.z(), 1.0f};
        return Vec3f{v.x(), v.y(), v.z()};
    }
    Vec3f TransformDirection(const Mat4f& m, const Vec3f& d)
    {
        Vec4f v = m * Vec4f{d.x(), d.y(), d.z(), 0.0f};
        Vec3f out{v.x(), v.y(), v.z()};

        if(lengthSquared(out) <= kEpsilonSq<float>)
        {
            return Vec3f{0.0f, 0.0f, 1.0f};
        }

        return normalize(out);
    }

    // LookAt / OpenGL / Right-Handed
    Mat4f LookAtOpenGLRH(const Vec3f& eye, const Vec3f& target, const Vec3f& up)
    {
        Vec3f fRaw = target - eye;
        assert(lengthSquared(fRaw) > kEpsilonSq<float>);

        Vec3f f = normalize(fRaw);
        assert(lengthSquared(f) > kEpsilonSq<float>);

        Vec3f s = normalize(cross(f, up)); // create fallback to avoid devide by 0
        assert(lengthSquared(s) > kEpsilonSq<float>);

        Vec3f u = cross(s, f);

        Mat4f m{};

        m(0, 0) = s[0];
        m(0, 1) = s[1];
        m(0, 2) = s[2];
        m(0, 3) = -dot(s, eye);
        m(1, 0) = u[0];
        m(1, 1) = u[1];
        m(1, 2) = u[2];
        m(1, 3) = -dot(u, eye);
        m(2, 0) = -f[0];
        m(2, 1) = -f[1];
        m(2, 2) = -f[2];
        m(2, 3) = dot(f, eye);
        m(3, 0) = 0;
        m(3, 1) = 0;
        m(3, 2) = 0;
        m(3, 3) = 1;

        return m;
    }

    // Perspective / OpenGL / Right-Handed
    Mat4f PerspectiveOpenGLRH(float fovYRad, float aspect, float nearZ, float farZ)
    {
        assert(aspect != 0.0f);
        assert(nearZ != farZ);
        assert(nearZ > 0.0f);
        assert(farZ > nearZ);

        float f = 1.0f / std::tan(fovYRad * 0.5f);

        Mat4f m{};

        m(0, 0) = f / aspect;
        m(1, 1) = f;
        m(2, 2) = (farZ + nearZ) / (nearZ - farZ);
        m(2, 3) = (2 * farZ * nearZ) / (nearZ - farZ);
        m(3, 2) = -1.0f;

        return m;
    }

    // Perspective / OpenGL / Right-Handed
    Mat4f OrthographicOpenGLRH(float left, float right, float bottom, float top, float nearZ, float farZ) 
    {
        assert(std::abs(right - left) > kEpsilon<float>);
        assert(std::abs(top - bottom) > kEpsilon<float>);
        assert(std::abs(farZ - nearZ) > kEpsilon<float>);

        Mat4f m{};

        m(0,0) = 2.0f / (right - left);
        m(1,1) = 2.0f / (top - bottom);
        m(2,2) = -2.0f / (farZ - nearZ);

        m(0,3) = -(right + left) / (right - left);
        m(1,3) = -(top + bottom) / (top - bottom);
        m(2,3) = -(farZ + nearZ) / (farZ - nearZ);

        m(3,3) = 1.0f;

        return m;
    }
} // namespace ddknd::math