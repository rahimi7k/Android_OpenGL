#ifndef VERTEX_H
#define VERTEX_H

#include "Vector2.h"
#include "Vector3.h"
#include <vector>
#include <array>

class Vertex {
public:
    Vector3 position;
    Vector2 texCoords;
    //Vector3 color;

    Vertex(float x = 0, float y = 0, float z = 0, float tu = 0, float tv = 0/*, float r = 0, float g= 0, float b= 0, float a= 0*/) : position(x, y, z), texCoords(tu, tv)/*, color(r, g, b)*/  {
    
    }

    std::vector<float> toArray() const {
        std::vector<float> array;
        std::vector<float> pos = position.toArray();
        std::vector<float> tex = texCoords.toArray();
		//std::vector<float> cl = {color.x, color.y, color.z, 1};

        array.insert(array.end(), pos.begin(), pos.end());
        array.insert(array.end(), tex.begin(), tex.end());
      //array.insert(array.end(), cl.begin(), cl.end());

        return array;
    }

    std::array<float, 5> toFloat32Array() const { //TODO
        std::array<float, 5> result;
        auto pos = position.toFloat32Array();
        auto tex = texCoords.toFloat32Array();
        std::copy(pos.begin(), pos.end(), result.begin());
        std::copy(tex.begin(), tex.end(), result.begin() + 3);
        return result;
    }
};

#endif // VERTEX_H