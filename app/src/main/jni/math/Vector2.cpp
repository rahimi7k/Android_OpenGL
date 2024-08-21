#include "Vector3.h"

Vector3 Vector2::toVector3() const {
    return Vector3(x, y, 0);
}
