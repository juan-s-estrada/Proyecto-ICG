#version 400

varying vec3 vNormal;
varying vec4 rPos;

uniform vec3 LightPos1;
uniform vec3 LightPos2;
uniform vec3 LightCol1;
uniform vec3 LightCol2;
uniform vec3 LightColS1;
uniform vec3 LightColS2;


uniform vec3 diffColour;
uniform vec3 lightColour;
uniform vec3 ambientColour;
uniform float ObjShininess;
uniform vec3 ObjSpecular;
void main()
{
vec3 N = normalize(vNormal);
vec3 L1 =  normalize(LightPos1-rPos.xyz);
vec3 L2 = normalize(LightPos2-rPos.xyz);
vec3 E = normalize(-rPos.xyz);

vec3 HV1 = normalize (E + L1);
vec3 HV2 = normalize (E + L2);

float R1 = dot(HV1,N);
float R2 = dot(HV2,N);

vec3 DiffComponent1 = diffColour *LightCol1 * max(dot(L1,N),0.0);
DiffComponent1 = clamp(DiffComponent1,0.0,1.0);

vec3 DiffComponent2 = diffColour *LightCol2 * max(dot(L2,N),0.0);
DiffComponent2 = clamp(DiffComponent2,0.0,1.0);
vec4 Ispec1 = vec4(0.0,0.0,0.0,0.0);
vec4 Ispec2 = vec4(0.0,0.0,0.0,0.0);

Ispec1 = vec4(LightColS1,1.0) * vec4(ObjSpecular,1.0) * pow(clamp(R1,0.0,1.0),ObjShininess);

Ispec2 = vec4(LightColS2,1.0) * vec4(ObjSpecular,1.0) * pow(clamp(R2,0.0,1.0),ObjShininess);

vec4 Ispec = clamp(Ispec1+Ispec2,0.0,1.0); 


gl_FragColor = vec4(ambientColour,1.0) + vec4(DiffComponent1,1.0) +vec4(DiffComponent2,1.0) + Ispec;
//gl_FragColor=vec4(0.5,0.5,0.5,0.5);


}