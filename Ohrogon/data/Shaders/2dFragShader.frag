#version 450

in vec3 position;
in vec2 TexCoord;


in vec3 color; 
uniform sampler2D DiffuseTexture;

out vec4 FragColor;


void main(){
  vec4 texCol = texture(DiffuseTexture, TexCoord);

  FragColor = vec4(color * texCol, 1);
}