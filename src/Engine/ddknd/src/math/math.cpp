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
} // namespace ddknd::math