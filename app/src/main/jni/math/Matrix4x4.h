//
// Created by Korosh on 7/8/2024.
//

#ifndef GAME_MATRIX4X4_H
#define GAME_MATRIX4X4_H

#include <vector>
#include <cmath>
#include "Vector3.h"
#include <GLES3/gl3.h>

class Matrix4x4 {
private:
    std::vector<float> _data;

    Matrix4x4() {
        _data = {
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
        };
    }

public:


    std::vector<float>& data() {
        return _data;
    }

    static Matrix4x4 identity() {
        return Matrix4x4();
    }

    static Matrix4x4 orthographic(float left, float right, float bottom, float top, float near, float far) {
        Matrix4x4 m;

        float rl = 1.0f / (right - left);
        float bt = 1.0f / (top - bottom);
        float nf = 1.0f / (far - near);

        float x = (right + left) * rl;
        float y = (top + bottom) * bt;
        float z = (far + near) * nf;

		m._data = {
				2 * rl, 0, 0, 0,
				0, 2 * bt, 0, 0,
				0, 0, -2 * nf, 0,
				-x, -y, -z, 1
		};

        return m;
    }









    static Matrix4x4 perspective(float left, float right, float top, float bottom, float near, float far) {
        Matrix4x4 m;

        float x = 2.0f * near / (right - left);
        float y = 2.0f * near / (top - bottom);

        float a = (right + left) / (right - left);
        float b = (top + bottom) / (top - bottom);

        float c = -(far + near) / (far - near);
        float d = -2.0f * far * near / (far - near);

        m._data = {
                x, 0, a, 0,
                0, y, b, 0,
                0, 0, c, d,
                0, 0, -1, 0
        };

        return m;
    }

    static Matrix4x4 translation(const Vector3& position) {
        Matrix4x4 m;

        m._data[12] = position.x;
        m._data[13] = position.y;
        m._data[14] = position.z;

        return m;
    }

    static Matrix4x4 rotationX(float angleInRadians) {
        Matrix4x4 m;

        float c = std::cos(angleInRadians);
        float s = std::sin(angleInRadians);

        m._data[5] = c;
        m._data[6] = s;
        m._data[9] = -s;
        m._data[10] = c;

        return m;
    }

    static Matrix4x4 rotationY(float angleInRadians) {
        Matrix4x4 m;

        float c = std::cos(angleInRadians);
        float s = std::sin(angleInRadians);

        m._data[0] = c;
        m._data[2] = -s;
        m._data[8] = s;
        m._data[10] = c;

        return m;
    }

    static Matrix4x4 rotationZ(float angleInRadians) {
        Matrix4x4 m;

        float c = std::cos(angleInRadians);
        float s = std::sin(angleInRadians);

        m._data[0] = c;
        m._data[1] = s;
        m._data[4] = -s;
        m._data[5] = c;

        return m;
    }

    static Matrix4x4 rotationXYZ(float xRadians, float yRadians, float zRadians) {
        Matrix4x4 rx = rotationX(xRadians);
        Matrix4x4 ry = rotationY(yRadians);
        Matrix4x4 rz = rotationZ(zRadians);

        return multiply(multiply(rz, ry), rx);
    }

    static Matrix4x4 scale(const Vector3& scale) {
        Matrix4x4 m;

        m._data[0] = scale.x;
        m._data[5] = scale.y;
        m._data[10] = scale.z;

        return m;
    }

    static Matrix4x4 multiply(const Matrix4x4& a, const Matrix4x4& b) {
        Matrix4x4 m;

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                m._data[i * 4 + j] =
                        a._data[i * 4 + 0] * b._data[0 * 4 + j] +
                        a._data[i * 4 + 1] * b._data[1 * 4 + j] +
                        a._data[i * 4 + 2] * b._data[2 * 4 + j] +
                        a._data[i * 4 + 3] * b._data[3 * 4 + j];
            }
        }

        return m;
    }

    const GLfloat* toFloat32Array() const {
        return _data.data();
    }

    void copyFrom(const Matrix4x4& m) {
        _data = m._data;
    }
};




#endif //GAME_MATRIX4X4_H
