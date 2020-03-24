#version 450

layout(location = 0) in vec3 VertPos;
layout(location = 1) in vec2 VertUV;

uniform mat4 MVPMatrix;
uniform mat4 ModelMatrix;

out vec2 TexCoord;
out vec3 position;

void main(){
	gl_Position = MVPMatrix * vec4(VertPos, 1);
	TexCoord = VertUV;
	position = (ModelMatrix * vec4(VertPos,1)).xyz;
}