#version 450

in vec3 position;
in vec2 TexCoord;
in vec3 normal;
in vec3 color;
in vec3 Tangent;
in vec3 BiTangent;

in mat3 TBN;

uniform PointLight{
  vec3 color;
  float intensity;
  vec3 position;
  float radius;
} PointLights[8];
uniform int PointLightCount;

uniform DirectionLight{
  vec3 color;
  float intensity;
  vec3 direction;
} DirectionLights[4];
uniform int DirectionLightCount;

uniform Material{
    vec3 Ka; // ambient material colour
    vec3 Kd; // diffuse material colour
    vec3 Ks; // specular material colour
    vec3 Ia; // ambient light colour
    vec3 Id; // diffuse light colour
    vec3 Is; // specular light colour
    vec3 LightDirection;
    float specularPower; // material specular power
} mat;

// uniform ColorBlock {
//     vec4 diffuse;
//     vec4 ambient;
// };

//uniform PhongMaterial Material;
uniform vec3 cameraPosition;

uniform sampler2D DiffuseTexture;
uniform sampler2D NormalTexture;

out vec4 FragColor;


vec3 DirectionLightColor(vec3 NormalDirection){
  vec3 DiffuseColor = vec3(0);
  for(int i = 0; i < DirectionLightCount; ++i){

    vec3 LightDirection = normalize(DirectionLights[i].direction);
    
    // calculate lambert term (negate light direction)
    float lambertTerm = max( 0, dot( NormalDirection, -LightDirection));

    // // calculate view vector and reflection vector
    // vec3 ViewDirection = normalize(cameraPosition - position);

    // vec3 Reflection = reflect(LightDirection, NormalDirection) * clamp(sign(dot(NormalDirection, -LightDirection)), 0, 1);
    // // calculate specular term
    // Specular += pow( max(0, dot(Reflection, ViewDirection) ), mat.specularPower) * DirectionLights[i].color;

    DiffuseColor += lambertTerm * (DirectionLights[i].color * DirectionLights[i].intensity);
  }
  return DiffuseColor;
}

vec3 PointLightColor(vec3 NormalDirection){
  vec3 DiffuseColor = vec3(0);
  for(int i = 0; i < PointLightCount; ++i){
    
    //calculate the vector from this pixels surface to the light source
    vec3 surfaceToLight = PointLights[i].position - position;

    //calculate the cosine of the angle of incidence
    float brightness = dot(NormalDirection, surfaceToLight) / (length(surfaceToLight) * length(NormalDirection));
    brightness = clamp(brightness, 0, 1);

    DiffuseColor += (PointLights[i].color * PointLights[i].intensity) * brightness;
  }
  return DiffuseColor;
}



void main(){
  vec4 texCol = texture(DiffuseTexture, TexCoord);
  vec4 normCol = texture(NormalTexture, TexCoord);
    
  vec3 ambient = mat.Ia * mat.Ka;

  vec3 TransformedNormal = normalize(TBN * ((normCol.xyz) * 2 - 1));

  vec3 Diffuse = DirectionLightColor(TransformedNormal);
  Diffuse += PointLightColor(TransformedNormal);

  // //ensure normal and light direction are normalised
  // vec3 N = normalDir;

  // // calculate each colour property

  // vec3 Specular = vec3(0);
  // vec3 Lighting = vec3(0);

  // for(int i = 0; i < DirectionLightCount; ++i){
  //   vec3 L = normalize(DirectionLights[i].direction);
  //   // calculate lambert term (negate light direction)
  //   float lambertTerm = max( 0, dot( N, -L ) );
  //   // calculate view vector and reflection vector
  //   vec3 V = normalize(cameraPosition - position);
  //   vec3 R = reflect( L, N ) * clamp(sign(dot(N, -L)), 0, 1);
  //   // calculate specular term
  //   Specular += pow( max(0, dot(R, V) ), mat.specularPower) * DirectionLights[i].color;
  //   Lighting += lambertTerm * (DirectionLights[i].color * DirectionLights[i].intensity);
  // }

  // vec3 diffuse = mat.Id * mat.Kd  * lambertTerm;
  // vec3 specular = mat.Is * mat.Ks * specularTerm;

  FragColor = vec4(Diffuse, 1) + 0.0001 * vec4((Diffuse + ambient) * texCol.xyz, 1);// + 0.0001 * vec4((texCol.xyz * clamp(ambient + diffuse, 0, 1)) + specular, 1);
}



/*
  vec3 finalColor = vec3(0);
  for(int i = 0; i < PointLightCount; ++i){
    finalColor += PointLights[i].color;
  }
*/