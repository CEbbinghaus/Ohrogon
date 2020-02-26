#version 450

in vec3 position;
in vec2 TexCoord;
in vec3 normal;
in vec3 color;

uniform Material{
    vec4 Ka; // ambient material colour
    vec4 Kd; // diffuse material colour
    vec4 Ks; // specular material colour
    vec4 Ia; // ambient light colour
    vec4 Id; // diffuse light colour
    vec4 Is; // specular light colour
    vec4 LightDirection;
    float specularPower; // material specular power
};

//uniform PhongMaterial Material;
uniform vec3 cameraPosition;

uniform sampler2D Texture;

out vec4 FragColor;

void main(){
    // ensure normal and light direction are normalised
    vec4 N = vec4(normalize(normal), 0);
    vec4 L = normalize(LightDirection);
    // calculate lambert term (negate light direction)
    float lambertTerm = max( 0, dot( N, -L ) );
    // calculate view vector and reflection vector
    vec4 V = normalize(cameraPosition -position);
    vec4 R = reflect( L, N );
    // calculate specular term
    float specularTerm = pow( max( 0, dot( R, V ) ), specularPower );
    // calculate each colour property
    vec4 ambient = vec3(0);//Ia * Ka;
    vec4 diffuse = vec3(lambertTerm);
    vec4 specular = Is * Ks * specularTerm;

    FragColor = ambient + diffuse + specular * 0.001 + vec4(L + vec3(0.5), 1);
}