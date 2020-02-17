#version 450

layout(location = 0) in vec3 VertPos;
layout(location = 1) in vec3 VertNormal;

uniform mat4 MVPMatrix;
uniform mat4 ModelMatrix;

out vec3 normal;
out vec3 color;

void main(){
	gl_Position = MVPMatrix * vec4(VertPos, 1);
	normal = normalize((ModelMatrix * vec4(VertNormal, 0)).xyz);
	color = vec3(1);
}