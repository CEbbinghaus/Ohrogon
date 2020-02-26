#version 450

in vec3 position;
in vec2 TexCoord;
in vec3 normal;
in vec3 color;

uniform Material{
    float Ka; // ambient material colour
    vec2 Kd; // diffuse material colour
    vec3 Ks; // specular material colour
    vec4 Ia; // ambient light colour
    mat3 Id; // diffuse light colour
    mat4 Is; // specular light colour
    mat2 LightDirection;
    float specularPower; // material specular power
};

//uniform PhongMaterial Material;
uniform vec3 cameraPosition;

uniform sampler2D Texture;

out vec4 FragColor;

void main(){
    vec4 t = vec4(0);
    t.x = Ka;
    t.xy += Kd;
    t.xyz += Ks;
    t += Ia;
    t.xyz += Id[0];
    t += Is[0];
    t.xy += LightDirection[0];
    t.x += specularPower;
    // ensure normal and light direction are normalised
    // vec3 N = normalize(normal);
    // vec3 L = normalize(LightDirection);
    // // calculate lambert term (negate light direction)
    // float lambertTerm = max( 0, dot( N, -L ) );
    // // calculate view vector and reflection vector
    // vec3 V = normalize(cameraPosition - position);
    // vec3 R = reflect( L, N );
    // // calculate specular term
    // float specularTerm = pow( max( 0, dot( R, V ) ), specularPower );
    // // calculate each colour property
    // vec3 ambient = vec3(0);//Ia * Ka;
    // vec3 diffuse = vec3(lambertTerm);
    // vec3 specular = Is * Ks * specularTerm;

    FragColor = t;//vec4();vec4( ambient + diffuse + specular, 1) * 0.001 + vec4(LightDirection, 1);
}