#version 450

layout(location = 0) in vec3 VertPos;
layout(location = 1) in vec3 VertNormal;
layout(location = 2) in vec2 VertUV;
layout(location = 3) in vec3 VertTangent;
layout(location = 4) in vec3 VertBiTangent;

uniform mat4 MVPMatrix;
uniform mat4 ModelMatrix;

out vec3 normal;
out vec3 color;
out vec2 TexCoord;
out vec3 position;
out vec3 Tangent;
out vec3 BiTangent;

void main(){
	gl_Position = MVPMatrix * vec4(VertPos, 1);
	normal = normalize((ModelMatrix * vec4(VertNormal, 0)).xyz);
	color = vec3(1);
	TexCoord = VertUV;
	position = (ModelMatrix * vec4(VertPos,1)).xyz;
	Tangent = VertTangent;
	BiTangent = VertBiTangent;
}