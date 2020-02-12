#version 450

in vec3 Color;
in vec4 gl_FragCoord;
out vec4 FragColor;

void main(){
    FragColor = vec4(Color + vec3(0.5), 1);//vec4(gl_FragCoord.xy * 0.002, 0, 0);
}