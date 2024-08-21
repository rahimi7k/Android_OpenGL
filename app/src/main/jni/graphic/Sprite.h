#ifndef SPRITE_H
#define SPRITE_H

#include <string>
#include <vector>
#include <GLES2/gl2.h>
#include "../gl/Buffer.h"
#include "../gl/Shader.h"
//#include "Material.h"
#include "../math/Vector3.h"
#include "../math/Vertex.h"
#include "../math/Matrix4x4.h"
#include "../Log.h"
#include "Texture.h"


class Sprite {
public:
    Sprite(const std::string& materialName, float width = 100.0f, float height = 100.0f);
    ~Sprite();

    void load();
    void update(float time);
    void draw(Shader& shader, const Matrix4x4& worldMatrix);
    void destroy();

    float getWidth() const;
    float getHeight() const;
    const Vector3& getOrigin() const;

    void setWidth(float width);
    void setHeight(float height);
    void setOrigin(float x, float y, float z);
    void setWidthAndHeight(float width, float height);

	void loadTexture(const char* name, int x, int y, int widthImg, int heightImg);


private:
    void recalculate();

    std::string _materialName;
    float _width;
    float _height;
    float _aspectRatio;


    Vector3 _origin;
    Buffer _buffer;
    std::vector<float> _vertices;

	Texture _texture;


	GLint _modelLocation;
    GLint _colorLocation;
    GLint _textureLocation;

};

#endif // SPRITE_H