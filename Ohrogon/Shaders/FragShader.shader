#version 450

in vec3 normal;
in vec3 color;
in vec4 gl_FragCoord;
out vec4 FragColor;

void main(){


    //PixelColor = color + vec3(0.5);//vec4(gl_FragCoord.xy * 0.002, 0, 0);

    vec3 ScreenColor = vec3(1);//vec3(gl_FragCoord.xy * 0.002, 0);

    vec3 PixelColor = (ScreenColor * normal * color) + vec3(0.5);

	vec3 LitPixel = PixelColor * vec3(0.7);

    FragColor = vec4(LitPixel, 1);
}