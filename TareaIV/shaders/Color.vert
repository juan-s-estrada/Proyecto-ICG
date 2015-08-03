#version 400

layout (location = 0) in vec3 VertexPosition;
varying vec4 Color;
uniform vec3 color;
uniform mat4 MVP;

void main()
{	
	Color=vec4(color,1);
	gl_Position = MVP * vec4(VertexPosition, 1.0);
}