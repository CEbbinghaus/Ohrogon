#include <atyp_Vector3.h>

float hue2rgb(float v1, float v2, float vH){
	if (vH < 0)
		vH += 1;

	if (vH > 1)
		vH -= 1;

	if ((6 * vH) < 1)
		return (v1 + (v2 - v1) * 6 * vH);

	if ((2 * vH) < 1)
		return v2;

	if ((3 * vH) < 2)
		return (v1 + (v2 - v1) * ((2.0f / 3) - vH) * 6);

	return v1;
}

Vector3 HSLtoRGB(float h, float s = 1.0f, float l = .5f){
    Vector3 rgb;

    if(s == 0.0f){
        rgb = Vector3::one() * l;
    }else{
        float v1, v2;
		float hue = (float)h / 360.0f;

		v2 = (l < 0.5) ? (l * (1 + s)) : ((l + s) - (l * s));
		v1 = 2 * l - v2;
        rgb.colRGB.r = hue2rgb(v1, v2, h + 1.0f/3);
        rgb.colRGB.g = hue2rgb(v1, v2, h);
        rgb.colRGB.b = hue2rgb(v1, v2, h - 1.0f/3);
    }
    return rgb;
}