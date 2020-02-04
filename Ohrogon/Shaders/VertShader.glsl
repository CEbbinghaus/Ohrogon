#version 450


struct BaseLight{
	vec3 Position;
	vec4 Color;
	float Intensity;
};

layout(location = 0) in vec3 VertPos;

uniform mat4 mvpMatrix;
uniform mat4 ModelMatrix;

void main(){
	gl_Position = vec4(VertPos, 1) * mvpMatrix;
}