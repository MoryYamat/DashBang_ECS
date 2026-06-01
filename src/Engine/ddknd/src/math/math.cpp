#include "math/math.h"

#include <cmath>
#include <numbers>

namespace ddknd::math
{
    float degToRadf(float degree)
    {
        return degree * (std::numbers::pi_v<float> / 180.0f);
    }

    Vec3f ComputeForawrdVec(float yawRad, float pitchRad)
    {
        Vec3f forward;
        forward[0] = std::cos(yawRad) * std::cos(pitchRad);
        forward[1] = std::sin(pitchRad);
        forward[2] = std::sin(yawRad) * std::cos(pitchRad);

        return forward;
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
} // namespace ddknd::math