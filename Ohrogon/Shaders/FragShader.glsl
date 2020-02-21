#version 450

in vec2 TexCoord;
in vec3 normal;
in vec3 color;
in vec4 gl_FragCoord;
out vec4 FragColor;

uniform sampler2D Texture;

void main(){


    //PixelColor = color + vec3(0.5);//vec4(gl_FragCoord.xy * 0.002, 0, 0);

    vec3 ScreenColor = vec3(1);//vec3(gl_FragCoord.xy * 0.002, 0);

    vec4 TextureColor = texture(Texture, TexCoord * vec2(1, -1));

    //if (TextureColor.a > 0.0001)
    //    discard;

    vec3 PixelColor = /*TextureColor.xyz + */(ScreenColor * normal * color) + vec3(.5);// * vec3(0.1));

	vec3 LitPixel = PixelColor * vec3(0.7);

    FragColor = vec4(LitPixel, 1);
}