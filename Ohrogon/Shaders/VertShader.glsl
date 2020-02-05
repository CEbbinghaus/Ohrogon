#version 450

layout(location = 0) in vec3 VertPos;

 uniform mat4 MVPMatrix;
// uniform mat4 ModelMatrix;

void main(){
	gl_Position = MVPMatrix * vec4(VertPos, 1);
}