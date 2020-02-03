#pragma once
#include <memory>
#include "atyp_Array.h"
#include "atyp_Vector3.h"
#include "atyp_Vector2.h"
#include "atyp_Matrix4.h"

class Transform{


	void updateLocalTransform();
	void updateGlobalTransform();

public:


	Matrix4 globalTransform;
	Matrix4 localTransform;


	static Transform* root;


	atyp::Array<Transform*> children;

	Transform* Parent;
	void TransferParent(Transform*);
	void SetParent(Transform*);
	void SetParent(std::nullptr_t);

	Vector3 Position;
	Vector3 Scale;
	Vector3 Rotation;
	
	Transform();
	Transform(Matrix4);
	Transform(Vector3, Vector3, Vector3);
	~Transform();

	Transform operator *(Transform& other);
	Matrix4 operator *(Matrix4& other);

	operator Matrix4();
};

Matrix4 operator * (Matrix4& lhs, Transform& rhs);