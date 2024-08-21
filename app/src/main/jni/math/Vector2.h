#ifndef VECTOR2_H
#define VECTOR2_H

#include <cmath>
#include <vector>
#include <array>
#include <cstddef>
#include "Vector3.h"

class Vector3;

class Vector2 {
public:
    float x;
    float y;

    Vector2(float x = 0, float y = 0) : x(x), y(y) {
    
    }
    static Vector2 zero() {
        return Vector2(0, 0);
    }

    static Vector2 one() {
        return Vector2(1, 1);
    }

    static float distance(const Vector2& a, const Vector2& b) {
        Vector2 diff = a - b;
        return std::sqrt(diff.x * diff.x + diff.y * diff.y);
    }

    void copyFrom(const Vector2& v) {
        x = v.x;
        y = v.y;
    }

    std::vector<float> toArray() const {
        return { x, y };
    }

    std::array<float, 2> toFloat32Array() const { //TODO
        return { x, y };
    }


    void set(float x_ = 0, float y_ = 0) {
        x = x_;
        y = y_;
    }

    Vector2 operator+(const Vector2& v) const {
        return Vector2(x + v.x, y + v.y);
    }

    Vector2 operator-(const Vector2& v) const {
        return Vector2(x - v.x, y - v.y);
    }

    Vector2 operator*(const Vector2& v) const {
        return Vector2(x * v.x, y * v.y);
    }

    Vector2 operator/(const Vector2& v) const {
        return Vector2(x / v.x, y / v.y);
    }

    Vector2 scale(float scale) const {
        return Vector2(x * scale, y * scale);
    }

    Vector2 clone() const {
        return Vector2(x, y);
    }

    Vector3 toVector3() const;


};

#endif // VECTOR2_H