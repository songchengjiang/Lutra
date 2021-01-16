//
//  MathHelper.cpp
//  Editor
//
//  Created by JasonCheng on 2021/1/15.
//

#include "MathHelper.h"

namespace LutraEditor {

    static float determinant(const glm::mat4& m)
    {
        float a0 = m[0][0] * m[1][1] - m[0][1] * m[1][0];
        float a1 = m[0][0] * m[1][2] - m[0][2] * m[1][0];
        float a2 = m[0][0] * m[1][3] - m[0][3] * m[1][0];
        float a3 = m[0][1] * m[1][2] - m[0][2] * m[1][1];
        float a4 = m[0][1] * m[1][3] - m[0][3] * m[1][1];
        float a5 = m[0][2] * m[1][3] - m[0][3] * m[1][2];
        float b0 = m[2][0] * m[3][1] - m[2][1] * m[3][0];
        float b1 = m[2][0] * m[3][2] - m[2][2] * m[3][0];
        float b2 = m[2][0] * m[3][3] - m[2][3] * m[3][0];
        float b3 = m[2][1] * m[3][2] - m[2][2] * m[3][1];
        float b4 = m[2][1] * m[3][3] - m[2][3] * m[3][1];
        float b5 = m[2][2] * m[3][3] - m[2][3] * m[3][2];

        // Calculate the determinant.
        return (a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0);
    }

    bool MathHelper::decompose(const glm::mat4& m, glm::vec3* translation, glm::quat* rotation, glm::vec3* scale)
    {
        if (translation)
        {
            // Extract the translation.
            translation->x = m[3][0];
            translation->y = m[3][1];
            translation->z = m[3][2];
        }

        // Nothing left to do.
        if (scale == nullptr && rotation == nullptr)
            return true;

        // Extract the scale.
        // This is simply the length of each axis (row/column) in the matrix.
        glm::vec3 xaxis(m[0][0], m[0][1], m[0][2]);
        float scaleX = glm::length(xaxis);

        glm::vec3 yaxis(m[1][0], m[1][1], m[1][2]);
        float scaleY = glm::length(yaxis);

        glm::vec3 zaxis(m[2][0], m[2][1], m[2][2]);
        float scaleZ = glm::length(zaxis);

        // Determine if we have a negative scale (true if determinant is less than zero).
        // In this case, we simply negate a single axis of the scale.
        float det = determinant(m);
        if (det < 0)
            scaleZ = -scaleZ;

        if (scale)
        {
            scale->x = scaleX;
            scale->y = scaleY;
            scale->z = scaleZ;
        }

        // Nothing left to do.
        if (rotation == nullptr)
            return true;

        // Scale too close to zero, can't decompose rotation.
        if (scaleX < 0.0001f || scaleY < 0.0001f || std::abs(scaleZ) < 0.0001f)
            return false;

        float rn;

        // Factor the scale out of the matrix axes.
        rn = 1.0f / scaleX;
        xaxis.x *= rn;
        xaxis.y *= rn;
        xaxis.z *= rn;

        rn = 1.0f / scaleY;
        yaxis.x *= rn;
        yaxis.y *= rn;
        yaxis.z *= rn;

        rn = 1.0f / scaleZ;
        zaxis.x *= rn;
        zaxis.y *= rn;
        zaxis.z *= rn;

        // Now calculate the rotation from the resulting matrix (axes).
        float trace = xaxis.x + yaxis.y + zaxis.z + 1.0f;

        if (trace > 0.0001f)
        {
            float s = 0.5f / std::sqrt(trace);
            rotation->w = 0.25f / s;
            rotation->x = (yaxis.z - zaxis.y) * s;
            rotation->y = (zaxis.x - xaxis.z) * s;
            rotation->z = (xaxis.y - yaxis.x) * s;
        }
        else
        {
            // Note: since xaxis, yaxis, and zaxis are normalized,
            // we will never divide by zero in the code below.
            if (xaxis.x > yaxis.y && xaxis.x > zaxis.z)
            {
                float s = 0.5f / std::sqrt(1.0f + xaxis.x - yaxis.y - zaxis.z);
                rotation->w = (yaxis.z - zaxis.y) * s;
                rotation->x = 0.25f / s;
                rotation->y = (yaxis.x + xaxis.y) * s;
                rotation->z = (zaxis.x + xaxis.z) * s;
            }
            else if (yaxis.y > zaxis.z)
            {
                float s = 0.5f / std::sqrt(1.0f + yaxis.y - xaxis.x - zaxis.z);
                rotation->w = (zaxis.x - xaxis.z) * s;
                rotation->x = (yaxis.x + xaxis.y) * s;
                rotation->y = 0.25f / s;
                rotation->z = (zaxis.y + yaxis.z) * s;
            }
            else
            {
                float s = 0.5f / std::sqrt(1.0f + zaxis.z - xaxis.x - yaxis.y);
                rotation->w = (xaxis.y - yaxis.x ) * s;
                rotation->x = (zaxis.x + xaxis.z ) * s;
                rotation->y = (zaxis.y + yaxis.z ) * s;
                rotation->z = 0.25f / s;
            }
        }

        return true;
    }

}
