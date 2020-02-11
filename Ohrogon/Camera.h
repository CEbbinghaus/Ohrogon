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
	
	CameraType type;
	
	Matrix4 getViewMatrix(){
		return (*(Matrix4*)&transform.updateTransform()).Invert();
	}

	Matrix4 getProjectionMatrix(){
		return Matrix4::Projection(fov, aspectRatio, NearPlane, FarPlane);
	}

	Matrix4 getPVMatrix(){
		return getProjectionMatrix() * getViewMatrix();
	}
};

