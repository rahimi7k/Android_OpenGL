#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Matrix4x4.h"
#include "Vector3.h"

class Transform {
public:
	Vector3 position = Vector3::zero();
	Vector3 rotation = Vector3::zero();
	Vector3 scale = Vector3::one();

	void copyFrom(const Transform& transform) {
		position.copyFrom(transform.position);
		rotation.copyFrom(transform.rotation);
		scale.copyFrom(transform.scale);
	}

	Matrix4x4 getTransformationMatrix() const {
		Matrix4x4 translation = Matrix4x4::translation(position);
		Matrix4x4 rotationMatrix = Matrix4x4::rotationXYZ(rotation.x, rotation.y, rotation.z);
		Matrix4x4 scaleMatrix = Matrix4x4::scale(scale);

		// T * R * S -- standard order of transformations: translate, then rotate, and finally scale
		//return Matrix4x4::multiply(Matrix4x4::multiply(translation, rotationMatrix), scaleMatrix);

		return Matrix4x4::multiply(Matrix4x4::multiply(rotationMatrix, scaleMatrix), translation); //Best
		//return Matrix4x4::multiply(Matrix4x4::multiply(scaleMatrix, rotationMatrix), translation);


		//return Matrix4x4::multiply(translation, Matrix4x4::multiply(rotationMatrix, scaleMatrix));

	}

	void setPosition(float x, float y, float z) {
		position.x = x;
		position.y = y;
		position.z = z;

	}

	void setRotation(float x, float y, float z) {
		rotation.x = x;
		rotation.y = y;
		rotation.z = z;
	}

	void setScale(float x, float y, float z) {
		scale.x = x;
		scale.y = y;
		scale.z = z;
	}


        void setScale(float scale) {
          if (scale < 0.0f) {
            return;
          }
          this->scale.x = scale;
          this->scale.y = scale;
          this->scale.z = scale;
        }

        Vector3 getScale() {
          return scale;
        }

};

#endif // TRANSFORM_H