#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 Normal;

varying vec4 rPos;
varying vec3 vNormal;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 NM;

void main()
{	

   rPos  = MV * vec4(VertexPosition,1.0);       
   
   vec4 aux = NM * vec4(Normal,1.0) ;
   vNormal = aux.xyz;
   gl_Position = MVP * vec4(VertexPosition, 1.0);
	
}