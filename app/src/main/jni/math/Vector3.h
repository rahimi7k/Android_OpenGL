#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>
#include <vector>
#include <array>
#include <cstddef>
#include <map>
#include "Vector2.h"

class Vector2;

class Vector3 {
public:
    float x;
    float y;
    float z;

    Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {
    
    }

    static Vector3 zero() {
        return Vector3(0, 0, 0);
    }

    static Vector3 one() {
        return Vector3(1, 1, 1);
    }

    static float distance(const Vector3& a, const Vector3& b) {
        Vector3 diff = a - b;
        return std::sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
    }

    void copyFrom(const Vector3& v) {
        x = v.x;
        y = v.y;
        z = v.z;
    }

    std::vector<float> toArray() const {
        return { x, y, z };
    }

    std::array<float, 3> toFloat32Array() const { //TODO
        return { x, y, z };
    }

    void set(float x_ = 0, float y_ = 0, float z_ = 0) {
        x = x_;
        y = y_;
        z = z_;
    }

    bool equals(const Vector3& v) const {
        return (x == v.x && y == v.y && z == v.z);
    }


    Vector3 operator+(const Vector3& v) const {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }

    Vector3 operator-(const Vector3& v) const {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }

    Vector3 operator*(const Vector3& v) const {
        return Vector3(x * v.x, y * v.y, z * v.z);
    }

    Vector3 operator/(const Vector3& v) const {
        return Vector3(x / v.x, y / v.y, z / v.z);
    }

    Vector3 clone() const {
        return Vector3(x, y, z);
    }

    Vector2 toVector2() const;

};

#endif // VECTOR3_H