#version 450

in vec4 gl_FragCoord;
out vec4 FragColor;

void main(){
    FragColor = vec4(gl_FragCoord.xy * 0.002, 0, 0);
}