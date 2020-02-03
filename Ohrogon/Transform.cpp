#include "Transform.h"

Transform* Transform::root = nullptr;

void Transform::updateLocalTransform() {
	Matrix4 _pos;
	Matrix4 _scale;
	Matrix4 _rotx;
	Matrix4 _roty;
	Matrix4 _rotz;

	_pos.setPosition(Position.x, Position.y, Position.z);
	_rotx.setRotateX(Rotation.x);
	_roty.setRotateY(Rotation.y);
	_rotz.setRotateZ(Rotation.z);
	_scale.setScale(Scale.x, Scale.y, Scale.z);
	
	localTransform = _pos * (_rotx * _roty * _rotz) * _scale;
}

void Transform::updateGlobalTransform(){
	updateLocalTransform();

	if(Parent)
		globalTransform = (Parent->globalTransform) * localTransform;
	else
		globalTransform = localTransform;

}

void Transform::TransferParent(Transform* next){
	if(Parent)
		Parent->children.remove(this);

	Parent = next;
	
	if(Parent)
		Parent->children.push(this);
}

void Transform::SetParent(Transform* p){
	TransferParent(p);
}

void Transform::SetParent(std::nullptr_t){
	TransferParent(root);
}

Transform::Transform(){
	Parent = nullptr;
	SetParent(nullptr);
	Position = Vector3(0.0f, 0.0f, 0.0f);
	Scale = Vector3(1.0f, 1.0f, 1.0f);
	Rotation = Vector3(0.0f, 0.0f, 0.0f);
}

Transform::Transform( Vector3 pos, Vector3 size, Vector3 rot){
	Parent = nullptr;
	SetParent(nullptr);
	Position = pos;
	Scale = size;
	Rotation = rot;
}

Transform::Transform(Matrix4 origin){
	Parent = nullptr;
	SetParent(nullptr);
	Position = Vector3(0.0f, 0.0f, 0.0f);
	Scale = Vector3(1.0f, 1.0f, 1.0f);
	Rotation = Vector3(0.0f, 0.0f, 0.0f);
	globalTransform = origin;
}


Transform::~Transform(){
	if (Parent)
		Parent->children.remove(this);
}

Transform Transform::operator*(Transform & other)
{
	return Transform(globalTransform * other.localTransform);
}

Matrix4 Transform::operator*(Matrix4 & other)
{
	return Matrix4(globalTransform * other);
}

Transform::operator Matrix4(){
	return globalTransform;
}

Matrix4 operator*(Matrix4 & lhs, Transform & rhs)
{
	return Matrix4(lhs * rhs.globalTransform);
}