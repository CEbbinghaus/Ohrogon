#version 450

in vec3 position;
in vec2 TexCoord;
in vec3 normal;
in vec3 color;
// uniform ColorBlock {
//     vec3 diffuse;
//     vec3 ambient;
// };

uniform Material{
    vec3 Ka; // ambient material colour
    vec3 Kd; // diffuse material colour
    vec3 Ks; // specular material colour
    vec3 Ia; // ambient light colour
    vec3 Id; // diffuse light colour
    vec3 Is; // specular light colour
    vec3 LightDirection;
    float specularPower; // material specular power
};

// uniform ColorBlock {
//     vec4 diffuse;
//     vec4 ambient;
// };

//uniform PhongMaterial Material;
uniform vec3 cameraPosition;

uniform sampler2D Texture;

out vec4 FragColor;

void main(){

    
    //ensure normal and light direction are normalised
  vec3 N = normalize(normal);
  vec3 L = normalize(LightDirection);
  // calculate lambert term (negate light direction)
  float lambertTerm = max( 0, dot( N, -L ) );
  // calculate view vector and reflection vector
  vec3 V = normalize(cameraPosition - position);
  vec3 R = reflect( L, N );
  // calculate specular term
  float specularTerm = pow( max( 0, dot( R, V ) ), specularPower );
  // calculate each colour property
  vec3 ambient = Ia * Ka;
  vec3 diffuse = Id * Kd  * lambertTerm;
  vec3 specular = Is * Ks * specularTerm;

  FragColor = vec4(ambient + diffuse + specular, 1);
}