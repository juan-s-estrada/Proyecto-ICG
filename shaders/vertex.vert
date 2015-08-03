#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 Normal;

varying vec3 N;
varying vec3 v;
uniform mat4 MVP;

void main()
{
	v=vec3(gl_ModelViewMatrix * vec4(VertexPosition,1.0));
	N = normalize(gl_NormalMatrix *vec4(Normal,1.0));
	gl_Position = MVP * vec4(VertexPosition, 1.0);
}