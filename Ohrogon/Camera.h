#pragma once
#include "atyp_Vector3.h"
#include "atyp_Quaternion.h"
#include "atyp_Matrix4.h"
#include "atyp_Transform.h"

enum class CameraType{
	Orthographic,
	Perspective
};

class Camera{
public:
	float fov = 1.50f;

	float NearPlane = 0.50f;
	float FarPlane = 1000.0f;

	float aspectRatio = 1.0f;

	Transform transform;
	
	CameraType type = CameraType::Perspective;
	
	Matrix4 getViewMatrix(){
		return Matrix4::Invert(transform.updateTransform());
	}

	Matrix4 getProjectionMatrix(){
		switch(type){
			case CameraType::Orthographic:
				return Matrix4::Orthographic(fov * 10, fov * 10 * aspectRatio, NearPlane, FarPlane);
			case CameraType::Perspective:
				return Matrix4::Projection(fov, aspectRatio, NearPlane, FarPlane);
		}
		throw "Camera Type does not exist";
	}

	Matrix4 getPVMatrix(){
		return getProjectionMatrix() * getViewMatrix();
	}
};

